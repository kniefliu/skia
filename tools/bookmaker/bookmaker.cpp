/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "bookmaker.h"

DEFINE_string2(bmh, b, "", "Path to a *.bmh file or a directory.");
DEFINE_bool2(catalog, c, false, "Write example catalog.htm. (Requires -b -f -r)");
DEFINE_string2(examples, e, "", "File of fiddlecli input, usually fiddle.json (For now, disables -r -f -s)");
DEFINE_string2(fiddle, f, "", "File of fiddlecli output, usually fiddleout.json.");
DEFINE_string2(include, i, "", "Path to a *.h file or a directory.");
DEFINE_bool2(hack, k, false, "Do a find/replace hack to update all *.bmh files. (Requires -b)");
DEFINE_bool2(stdout, o, false, "Write file out to standard out.");
DEFINE_bool2(populate, p, false, "Populate include from bmh. (Requires -b -i)");
DEFINE_string2(ref, r, "", "Resolve refs and write bmh_*.md files to path. (Requires -b -f)");
DEFINE_string2(spellcheck, s, "", "Spell-check [once, all, mispelling]. (Requires -b)");
DEFINE_bool2(tokens, t, false, "Write bmh from include. (Requires -b -i)");
DEFINE_bool2(crosscheck, x, false, "Check bmh against includes. (Requires -b -i)");
DEFINE_bool2(skip, z, false, "Skip degenerate missed in legacy preprocessor.");

/*  recipe for generating timestamps for existing doxygen comments
find include/core -type f -name '*.h' -print -exec git blame {} \; > ~/all.blame.txt

space table better for Constants
should Return be on same line as 'Return Value'?
remove anonymous header, e.g. Enum SkPaint::::anonymous_2
Text Encoding anchors in paragraph are echoed instead of being linked to anchor names
    also should not point to 'undocumented' since they are resolvable links
#Member lost all formatting
#List needs '# content ##', formatting
consts like enum members need fully qualfied refs to make a valid link
enum comments should be disallowed unless after #Enum and before first #Const
    ... or, should look for enum comments in other places
trouble with aliases, plurals
    need to keep first letter of includeWriter @param / @return lowercase
    Quad -> quad, Quads -> quads
check for summary containing all methods
 */

/*
  class contains named struct, enum, enum-member, method, topic, subtopic
     everything contained by class is uniquely named
     contained names may be reused by other classes
  method contains named parameters
     parameters may be reused in other methods
 */

bool BmhParser::addDefinition(const char* defStart, bool hasEnd, MarkType markType,
        const vector<string>& typeNameBuilder) {
    Definition* definition = nullptr;
    switch (markType) {
        case MarkType::kComment:
            if (!this->skipToDefinitionEnd(markType)) {
                return false;
            }
            return true;
        // these types may be referred to by name
        case MarkType::kClass:
        case MarkType::kStruct:
        case MarkType::kConst:
        case MarkType::kEnum:
        case MarkType::kEnumClass:
        case MarkType::kMember:
        case MarkType::kMethod:
        case MarkType::kTypedef: {
            if (!typeNameBuilder.size()) {
                return this->reportError<bool>("unnamed markup");
            }
            if (typeNameBuilder.size() > 1) {
                return this->reportError<bool>("expected one name only");
            }
            const string& name = typeNameBuilder[0];
            if (nullptr == fRoot) {
                fRoot = this->findBmhObject(markType, name);
                fRoot->fFileName = fFileName;
                definition = fRoot;
            } else {
                if (nullptr == fParent) {
                    return this->reportError<bool>("expected parent");
                }
                if (fParent == fRoot && hasEnd) {
                    RootDefinition* rootParent = fRoot->rootParent();
                    if (rootParent) {
                        fRoot = rootParent;
                    }
                    definition = fParent;
                } else {
                    if (!hasEnd && fRoot->find(name, RootDefinition::AllowParens::kNo)) {
                        return this->reportError<bool>("duplicate symbol");
                    }
                    if (MarkType::kStruct == markType || MarkType::kClass == markType) {
                        // if class or struct, build fRoot hierarchy
                        // and change isDefined to search all parents of fRoot
                        SkASSERT(!hasEnd);
                        RootDefinition* childRoot = new RootDefinition;
                        (fRoot->fBranches)[name] = childRoot;
                        childRoot->setRootParent(fRoot);
                        childRoot->fFileName = fFileName;
                        fRoot = childRoot;
                        definition = fRoot;
                    } else {
                        definition = &fRoot->fLeaves[name];
                    }
                }
            }
            if (hasEnd) {
                Exemplary hasExample = Exemplary::kNo;
                bool hasExcluder = false;
                for (auto child : definition->fChildren) {
                     if (MarkType::kExample == child->fMarkType) {
                        hasExample = Exemplary::kYes;
                     }
                     hasExcluder |= MarkType::kPrivate == child->fMarkType
                            || MarkType::kDeprecated == child->fMarkType
                            || MarkType::kExperimental == child->fMarkType
                            || MarkType::kNoExample == child->fMarkType;
                }
                if (fMaps[(int) markType].fExemplary != hasExample
                        && fMaps[(int) markType].fExemplary != Exemplary::kOptional) {
                    if (string::npos == fFileName.find("undocumented")
                            && !hasExcluder) {
                        hasExample == Exemplary::kNo ?
                                this->reportWarning("missing example") :
                                this->reportWarning("unexpected example");
                    }

                }
                if (MarkType::kMethod == markType) {
                    if (fCheckMethods && !definition->checkMethod()) {
                        return false;
                    }
                }
                if (!this->popParentStack(definition)) {
                    return false;
                }
            } else {
                definition->fStart = defStart;
                this->skipSpace();
                definition->fFileName = fFileName;
                definition->fContentStart = fChar;
                definition->fLineCount = fLineCount;
                definition->fClone = fCloned;
                if (MarkType::kConst == markType) {
                    // todo: require that fChar points to def on same line as markup
                    // additionally add definition to class children if it is not already there
                    if (definition->fParent != fRoot) {
//                        fRoot->fChildren.push_back(definition);
                    }
                }
                definition->fName = name;
                if (MarkType::kMethod == markType) {
                    if (string::npos != name.find(':', 0)) {
                        definition->setCanonicalFiddle();
                    } else {
                        definition->fFiddle = name;
                    }
                } else {
                    definition->fFiddle = Definition::NormalizedName(name);
                }
                definition->fMarkType = markType;
                definition->fAnonymous = fAnonymous;
                this->setAsParent(definition);
            }
            } break;
        case MarkType::kTopic:
        case MarkType::kSubtopic:
            SkASSERT(1 == typeNameBuilder.size());
            if (!hasEnd) {
                if (!typeNameBuilder.size()) {
                    return this->reportError<bool>("unnamed topic");
                }
                fTopics.emplace_front(markType, defStart, fLineCount, fParent);
                RootDefinition* rootDefinition = &fTopics.front();
                definition = rootDefinition;
                definition->fFileName = fFileName;
                definition->fContentStart = fChar;
                definition->fName = typeNameBuilder[0];
                Definition* parent = fParent;
                while (parent && MarkType::kTopic != parent->fMarkType
                        && MarkType::kSubtopic != parent->fMarkType) {
                    parent = parent->fParent;
                }
                definition->fFiddle = parent ? parent->fFiddle + '_' : "";
                definition->fFiddle += Definition::NormalizedName(typeNameBuilder[0]);
                this->setAsParent(definition);
            }
            {
                const string& fullTopic = hasEnd ? fParent->fFiddle : definition->fFiddle;
                Definition* defPtr = fTopicMap[fullTopic];
                if (hasEnd) {
                    if (!definition) {
                        definition = defPtr;
                    } else if (definition != defPtr) {
                        return this->reportError<bool>("mismatched topic");
                    }
                } else {
                    if (nullptr != defPtr) {
                        return this->reportError<bool>("already declared topic");
                    }
                    fTopicMap[fullTopic] = definition;
                }
            }
            if (hasEnd) {
                if (!this->popParentStack(definition)) {
                    return false;
                }
            }
            break;
        // these types are children of parents, but are not in named maps
        case MarkType::kDefinedBy: {
            string prefixed(fRoot->fName);
            const char* start = fChar;
            string name(start, this->trimmedBracketEnd(fMC) - start);
            prefixed += "::" + name;
            this->skipToEndBracket(fMC);
            const auto leafIter = fRoot->fLeaves.find(prefixed);
            if (fRoot->fLeaves.end() != leafIter) {
                this->reportError<bool>("DefinedBy already defined");
            }
            definition = &fRoot->fLeaves[prefixed];
            definition->fParent = fParent;
            definition->fStart = defStart;
            definition->fContentStart = start;
            definition->fName = name;
            definition->fFiddle = Definition::NormalizedName(name);
            definition->fContentEnd = fChar;
            this->skipToEndBracket('\n');
            definition->fTerminator = fChar;
            definition->fMarkType = markType;
            definition->fLineCount = fLineCount;
            fParent->fChildren.push_back(definition);
            } break;
        case MarkType::kDescription:
        case MarkType::kStdOut:
        // may be one-liner
        case MarkType::kBug:
        case MarkType::kNoExample:
        case MarkType::kParam:
        case MarkType::kReturn:
        case MarkType::kToDo:
            if (hasEnd) {
                if (markType == fParent->fMarkType) {
                    definition = fParent;
                    if (MarkType::kBug == markType || MarkType::kReturn == markType
                            || MarkType::kToDo == markType) {
                        this->skipNoName();
                    }
                    if (!this->popParentStack(fParent)) { // if not one liner, pop
                        return false;
                    }
                    if (MarkType::kParam == markType || MarkType::kReturn == markType) {
                        if (!this->checkParamReturn(definition)) {
                            return false;
                        }
                    }
                } else {
                    fMarkup.emplace_front(markType, defStart, fLineCount, fParent);
                    definition = &fMarkup.front();
                    definition->fName = typeNameBuilder[0];
                    definition->fFiddle = fParent->fFiddle;
                    definition->fContentStart = fChar;
                    definition->fContentEnd = this->trimmedBracketEnd(fMC);
                    this->skipToEndBracket(fMC);
                    SkAssertResult(fMC == this->next());
                    SkAssertResult(fMC == this->next());
                    definition->fTerminator = fChar;
                    fParent->fChildren.push_back(definition);
                }
                break;
            }
        // not one-liners
        case MarkType::kCode:
        case MarkType::kDeprecated:
        case MarkType::kExample:
        case MarkType::kExperimental:
        case MarkType::kFormula:
        case MarkType::kFunction:
        case MarkType::kLegend:
        case MarkType::kList:
        case MarkType::kPrivate:
        case MarkType::kTable:
        case MarkType::kTrack:
            if (hasEnd) {
                definition = fParent;
                if (markType != fParent->fMarkType) {
                    return this->reportError<bool>("end element mismatch");
                } else if (!this->popParentStack(fParent)) {
                    return false;
                }
                if (MarkType::kExample == markType) {
                    if (definition->fChildren.size() == 0) {
                        TextParser emptyCheck(definition);
                        if (emptyCheck.eof() || !emptyCheck.skipWhiteSpace()) {
                            return this->reportError<bool>("missing example body");
                        }
                    }
                    definition->setWrapper();
                }
            } else {
                fMarkup.emplace_front(markType, defStart, fLineCount, fParent);
                definition = &fMarkup.front();
                definition->fContentStart = fChar;
                definition->fName = typeNameBuilder[0];
                definition->fFiddle = fParent->fFiddle;
                char suffix = '\0';
                bool tryAgain;
                do {
                    tryAgain = false;
                    for (const auto& child : fParent->fChildren) {
                        if (child->fFiddle == definition->fFiddle) {
                            if (MarkType::kExample != child->fMarkType) {
                                continue;
                            }
                            if ('\0' == suffix) {
                                suffix = 'a';
                            } else if (++suffix > 'z') {
                                return reportError<bool>("too many examples");
                            }
                            definition->fFiddle = fParent->fFiddle + '_';
                            definition->fFiddle += suffix;
                            tryAgain = true;
                            break;
                        }
                    }
                } while (tryAgain);
                this->setAsParent(definition);
            }
            break;
            // always treated as one-liners (can't detect misuse easily)
        case MarkType::kAlias:
        case MarkType::kAnchor:
        case MarkType::kDefine:
        case MarkType::kError:
        case MarkType::kFile:
        case MarkType::kHeight:
        case MarkType::kImage:
        case MarkType::kLiteral:
        case MarkType::kOutdent:
        case MarkType::kPlatform:
        case MarkType::kSeeAlso:
        case MarkType::kSubstitute:
        case MarkType::kTime:
        case MarkType::kVolatile:
        case MarkType::kWidth:
            if (hasEnd && MarkType::kAnchor != markType) {
                return this->reportError<bool>("one liners omit end element");
            } else if (!hasEnd && MarkType::kAnchor == markType) {
                return this->reportError<bool>("anchor line must have end element last");
            }
            fMarkup.emplace_front(markType, defStart, fLineCount, fParent);
            definition = &fMarkup.front();
            definition->fName = typeNameBuilder[0];
            definition->fFiddle = Definition::NormalizedName(typeNameBuilder[0]);
            definition->fContentStart = fChar;
            definition->fContentEnd = this->trimmedBracketEnd('\n');
            definition->fTerminator = this->lineEnd() - 1;
            fParent->fChildren.push_back(definition);
            if (MarkType::kAnchor == markType) {
                this->skipToEndBracket(fMC);
                fMarkup.emplace_front(MarkType::kLink, fChar, fLineCount, definition);
                SkAssertResult(fMC == this->next());
                this->skipWhiteSpace();
                Definition* link = &fMarkup.front();
                link->fContentStart = fChar;
                link->fContentEnd = this->trimmedBracketEnd(fMC);
                this->skipToEndBracket(fMC);
                SkAssertResult(fMC == this->next());
                SkAssertResult(fMC == this->next());
                link->fTerminator = fChar;
                definition->fContentEnd = link->fContentEnd;
                definition->fTerminator = fChar;
                definition->fChildren.emplace_back(link);
            } else if (MarkType::kAlias == markType) {
                this->skipWhiteSpace();
                const char* start = fChar;
                this->skipToNonAlphaNum();
                string alias(start, fChar - start);
                if (fAliasMap.end() != fAliasMap.find(alias)) {
                    return this->reportError<bool>("duplicate alias");
                }
                fAliasMap[alias] = definition;
            }
            break;
        case MarkType::kExternal:
            (void) this->collectExternals();  // FIXME: detect errors in external defs?
            break;
        default:
            SkASSERT(0);  // fixme : don't let any types be invisible
            return true;
    }
    if (fParent) {
        SkASSERT(definition);
        SkASSERT(definition->fName.length() > 0);
    }
    return true;
}

void BmhParser::reportDuplicates(const Definition& def, const string& dup) const {
    if (MarkType::kExample == def.fMarkType && dup == def.fFiddle) {
        TextParser reporter(&def);
        reporter.reportError("duplicate example name");
    }
    for (auto& child : def.fChildren ) {
        reportDuplicates(*child, dup);
    }
}

static void find_examples(const Definition& def, vector<string>* exampleNames) {
    if (MarkType::kExample == def.fMarkType) {
        exampleNames->push_back(def.fFiddle);
    }
    for (auto& child : def.fChildren ) {
        find_examples(*child, exampleNames);
    }
}

bool BmhParser::checkExamples() const {
    vector<string> exampleNames;
    for (const auto& topic : fTopicMap) {
        if (topic.second->fParent) {
            continue;
        }
        find_examples(*topic.second, &exampleNames);
    }
    std::sort(exampleNames.begin(), exampleNames.end());
    string* last = nullptr;
    string reported;
    bool checkOK = true;
    for (auto& nameIter : exampleNames) {
        if (last && *last == nameIter && reported != *last) {
            reported = *last;
            SkDebugf("%s\n", reported.c_str());
            for (const auto& topic : fTopicMap) {
                if (topic.second->fParent) {
                    continue;
                }
                this->reportDuplicates(*topic.second, reported);
            }
            checkOK = false;
        }
        last = &nameIter;
    }
    return checkOK;
}

bool BmhParser::checkParamReturn(const Definition* definition) const {
    const char* parmEndCheck = definition->fContentEnd;
    while (parmEndCheck < definition->fTerminator) {
        if (fMC == parmEndCheck[0]) {
            break;
        }
        if (' ' < parmEndCheck[0]) {
            this->reportError<bool>(
                    "use full end marker on multiline #Param and #Return");
        }
        ++parmEndCheck;
    }
    return true;
}

bool BmhParser::childOf(MarkType markType) const {
    auto childError = [this](MarkType markType) -> bool {
        string errStr = "expected ";
        errStr += fMaps[(int) markType].fName;
        errStr += " parent";
        return this->reportError<bool>(errStr.c_str());
    };

    if (markType == fParent->fMarkType) {
        return true;
    }
    if (this->hasEndToken()) {
        if (!fParent->fParent) {
            return this->reportError<bool>("expected grandparent");
        }
        if (markType == fParent->fParent->fMarkType) {
            return true;
        }
    }
    return childError(markType);
}

string BmhParser::className(MarkType markType) {
    const char* end = this->lineEnd();
    const char* mc = this->strnchr(fMC, end);
    string classID;
    TextParser::Save savePlace(this);
    this->skipSpace();
    const char* wordStart = fChar;
    this->skipToNonAlphaNum();
    const char* wordEnd = fChar;
    classID = string(wordStart, wordEnd - wordStart);
    if (!mc) {
        savePlace.restore();
    }
    string builder;
    const Definition* parent = this->parentSpace();
    if (parent && parent->fName != classID) {
        builder += parent->fName;
    }
    if (mc) {
        if (mc + 1 < fEnd && fMC == mc[1]) {  // if ##
            if (markType != fParent->fMarkType) {
                return this->reportError<string>("unbalanced method");
            }
            if (builder.length() > 0 && classID.size() > 0) {
                if (builder != fParent->fName) {
                    builder += "::";
                    builder += classID;
                    if (builder != fParent->fName) {
                        return this->reportError<string>("name mismatch");
                    }
                }
            }
            this->skipLine();
            return fParent->fName;
        }
        fChar = mc;
        this->next();
    }
    this->skipWhiteSpace();
    if (MarkType::kEnum == markType && fChar >= end) {
        fAnonymous = true;
        builder += "::_anonymous";
        return uniqueRootName(builder, markType);
    }
    builder = this->word(builder, "::");
    return builder;
}

bool BmhParser::collectExternals() {
    do {
        this->skipWhiteSpace();
        if (this->eof()) {
            break;
        }
        if (fMC == this->peek()) {
            this->next();
            if (this->eof()) {
                break;
            }
            if (fMC == this->peek()) {
                this->skipLine();
                break;
            }
            if (' ' >= this->peek()) {
                this->skipLine();
                continue;
            }
            if (this->startsWith(fMaps[(int) MarkType::kExternal].fName)) {
                this->skipToNonAlphaNum();
                continue;
            }
        }
        this->skipToAlpha();
        const char* wordStart = fChar;
        this->skipToNonAlphaNum();
        if (fChar - wordStart > 0) {
            fExternals.emplace_front(MarkType::kExternal, wordStart, fChar, fLineCount, fParent);
            RootDefinition* definition = &fExternals.front();
            definition->fFileName = fFileName;
            definition->fName = string(wordStart ,fChar - wordStart);
            definition->fFiddle = Definition::NormalizedName(definition->fName);
        }
    } while (!this->eof());
    return true;
}

static bool dump_examples(FILE* fiddleOut, const Definition& def, bool* continuation) {
    if (MarkType::kExample == def.fMarkType) {
        string result;
        if (!def.exampleToScript(&result, Definition::ExampleOptions::kAll)) {
            return false;
        }
        if (result.length() > 0) {
            result += "\n";
            result += "}";
            if (*continuation) {
                fprintf(fiddleOut, ",\n");
            } else {
                *continuation = true;
            }
            fprintf(fiddleOut, "%s", result.c_str());
        }
        return true;
    }
    for (auto& child : def.fChildren ) {
        if (!dump_examples(fiddleOut, *child, continuation)) {
            return false;
        }
    }
    return true;
}

bool BmhParser::dumpExamples(const char* fiddleJsonFileName) const {
    FILE* fiddleOut = fopen(fiddleJsonFileName, "wb");
    if (!fiddleOut) {
        SkDebugf("could not open output file %s\n", fiddleJsonFileName);
        return false;
    }
    fprintf(fiddleOut, "{\n");
    bool continuation = false;
    for (const auto& topic : fTopicMap) {
        if (topic.second->fParent) {
            continue;
        }
        dump_examples(fiddleOut, *topic.second, &continuation);
    }
    fprintf(fiddleOut, "\n}\n");
    fclose(fiddleOut);
    SkDebugf("wrote %s\n", fiddleJsonFileName);
    return true;
}

int BmhParser::endHashCount() const {
    const char* end = fLine + this->lineLength();
    int count = 0;
    while (fLine < end && fMC == *--end) {
        count++;
    }
    return count;
}

bool BmhParser::endTableColumn(const char* end, const char* terminator) {
    if (!this->popParentStack(fParent)) {
        return false;
    }
    fWorkingColumn->fContentEnd = end;
    fWorkingColumn->fTerminator = terminator;
    fColStart = fChar - 1;
    this->skipSpace();
    fTableState = TableState::kColumnStart;
    return true;
}

// FIXME: some examples may produce different output on different platforms
// if the text output can be different, think of how to author that

bool BmhParser::findDefinitions() {
    bool lineStart = true;
    const char* lastChar = nullptr;
    const char* lastMC = nullptr;
    fParent = nullptr;
    while (!this->eof()) {
        if (this->peek() == fMC) {
            lastMC = fChar;
            this->next();
            if (this->peek() == fMC) {
                this->next();
                if (!lineStart && ' ' < this->peek()) {
                    return this->reportError<bool>("expected definition");
                }
                if (this->peek() != fMC) {
                    if (MarkType::kColumn == fParent->fMarkType) {
                        SkASSERT(TableState::kColumnEnd == fTableState);
                        if (!this->endTableColumn(lastChar, lastMC)) {
                            return false;
                        }
                        SkASSERT(fRow);
                        if (!this->popParentStack(fParent)) {
                            return false;
                        }
                        fRow->fContentEnd = fWorkingColumn->fContentEnd;
                        fWorkingColumn = nullptr;
                        fRow = nullptr;
                        fTableState = TableState::kNone;
                    } else {
                        vector<string> parentName;
                        parentName.push_back(fParent->fName);
                        if (!this->addDefinition(fChar - 1, true, fParent->fMarkType, parentName)) {
                            return false;
                        }
                    }
                } else {
                    SkAssertResult(this->next() == fMC);
                    fMC = this->next();  // change markup character
                    if (' ' >= fMC) {
                        return this->reportError<bool>("illegal markup character");
                    }
                    fMarkup.emplace_front(MarkType::kMarkChar, fChar - 1, fLineCount, fParent);
                    Definition* markChar = &fMarkup.front();
                    markChar->fContentStart = fChar - 1;
                    this->skipToEndBracket('\n');
                    markChar->fContentEnd = fChar;
                    markChar->fTerminator = fChar;
                    fParent->fChildren.push_back(markChar);
                }
            } else if (this->peek() >= 'A' && this->peek() <= 'Z') {
                const char* defStart = fChar - 1;
                MarkType markType = this->getMarkType(MarkLookup::kRequire);
                bool hasEnd = this->hasEndToken();
                if (!hasEnd) {
                    MarkType parentType = fParent ? fParent->fMarkType : MarkType::kRoot;
                    uint64_t parentMask = fMaps[(int) markType].fParentMask;
                    if (parentMask && !(parentMask & (1LL << (int) parentType))) {
                        return this->reportError<bool>("invalid parent");
                    }
                }
                if (!this->skipName(fMaps[(int) markType].fName)) {
                    return this->reportError<bool>("illegal markup character");
                }
                if (!this->skipSpace()) {
                    return this->reportError<bool>("unexpected end");
                }
                bool expectEnd = true;
                vector<string> typeNameBuilder = this->typeName(markType, &expectEnd);
                if (fCloned && MarkType::kMethod != markType && MarkType::kExample != markType
                        && !fAnonymous) {
                    return this->reportError<bool>("duplicate name");
                }
                if (hasEnd && expectEnd) {
                    SkASSERT(fMC != this->peek());
                }
                if (!this->addDefinition(defStart, hasEnd, markType, typeNameBuilder)) {
                    return false;
                }
                continue;
            } else if (this->peek() == ' ') {
                if (!fParent || (MarkType::kTable != fParent->fMarkType
                        && MarkType::kLegend != fParent->fMarkType
                        && MarkType::kList != fParent->fMarkType)) {
                    int endHashes = this->endHashCount();
                    if (endHashes <= 1) {
                        if (fParent) {
                            if (TableState::kColumnEnd == fTableState) {
                                if (!this->endTableColumn(lastChar, lastMC)) {
                                    return false;
                                }
                            } else {  // one line comment
                                fMarkup.emplace_front(MarkType::kComment, fChar - 1, fLineCount,
                                        fParent);
                                Definition* comment = &fMarkup.front();
                                comment->fContentStart = fChar - 1;
                                this->skipToEndBracket('\n');
                                comment->fContentEnd = fChar;
                                comment->fTerminator = fChar;
                                fParent->fChildren.push_back(comment);
                            }
                        } else {
                            fChar = fLine + this->lineLength() - 1;
                        }
                    } else {  // table row
                        if (2 != endHashes) {
                            string errorStr = "expect ";
                            errorStr += fMC;
                            errorStr += fMC;
                            return this->reportError<bool>(errorStr.c_str());
                        }
                        if (!fParent || MarkType::kTable != fParent->fMarkType) {
                            return this->reportError<bool>("missing table");
                        }
                    }
                } else if (TableState::kNone == fTableState) {
                    // fixme? no nested tables for now
                    fColStart = fChar - 1;
                    fMarkup.emplace_front(MarkType::kRow, fColStart, fLineCount, fParent);
                    fRow = &fMarkup.front();
                    fRow->fName = fParent->fName;
                    this->skipWhiteSpace();
                    fRow->fContentStart = fChar;
                    this->setAsParent(fRow);
                    fTableState = TableState::kColumnStart;
                }
                if (TableState::kColumnStart == fTableState) {
                    fMarkup.emplace_front(MarkType::kColumn, fColStart, fLineCount, fParent);
                    fWorkingColumn = &fMarkup.front();
                    fWorkingColumn->fName = fParent->fName;
                    fWorkingColumn->fContentStart = fChar;
                    this->setAsParent(fWorkingColumn);
                    fTableState = TableState::kColumnEnd;
                    continue;
                }
            }
        }
        char nextChar = this->next();
        lineStart = nextChar == '\n';
        if (' ' < nextChar) {
            lastChar = fChar;
        }
    }
    if (fParent) {
        return fParent->reportError<bool>("mismatched end");
    }
    return true;
}

MarkType BmhParser::getMarkType(MarkLookup lookup) const {
    for (int index = 0; index <= Last_MarkType; ++index) {
        int typeLen = strlen(fMaps[index].fName);
        if (typeLen == 0) {
            continue;
        }
        if (fChar + typeLen >= fEnd || fChar[typeLen] > ' ') {
            continue;
        }
        int chCompare = strncmp(fChar, fMaps[index].fName, typeLen);
        if (chCompare < 0) {
            goto fail;
        }
        if (chCompare == 0) {
            return (MarkType) index;
        }
    }
fail:
    if (MarkLookup::kRequire == lookup) {
        return this->reportError<MarkType>("unknown mark type");
    }
    return MarkType::kNone;
}

bool HackParser::hackFiles() {
    string filename(fFileName);
    size_t len = filename.length() - 1;
    while (len > 0 && (isalnum(filename[len]) || '_' == filename[len] || '.' == filename[len])) {
        --len;
    }
    filename = filename.substr(len + 1);
    // remove trailing period from #Param and #Return
    FILE* out = fopen(filename.c_str(), "wb");
    if (!out) {
        SkDebugf("could not open output file %s\n", filename.c_str());
        return false;
    }
    const char* start = fStart;
    do {
        const char* match = this->strnchr('#', fEnd);
        if (!match) {
            break;
        }
        this->skipTo(match);
        this->next();
        if (!this->startsWith("Param") && !this->startsWith("Return")) {
            continue;
        }
        const char* end = this->strnstr("##", fEnd);
        while (true) {
            TextParser::Save lastPeriod(this);
            this->next();
            if (!this->skipToEndBracket('.', end)) {
                lastPeriod.restore();
                break;
            }
        }
        if ('.' == this->peek()) {
            fprintf(out, "%.*s", (int) (fChar - start), start);
            this->next();
            start = fChar;
        }
    } while (!this->eof());
    fprintf(out, "%.*s", (int) (fEnd - start), start);
    fclose(out);
    SkDebugf("wrote %s\n", filename.c_str());
    return true;
}

bool BmhParser::hasEndToken() const {
    const char* last = fLine + this->lineLength();
    while (last > fLine && ' ' >= *--last)
        ;
    if (--last < fLine) {
        return false;
    }
    return last[0] == fMC && last[1] == fMC;
}

string BmhParser::memberName() {
    const char* wordStart;
    const char* prefixes[] = { "static", "const" };
    do {
        this->skipSpace();
        wordStart = fChar;
        this->skipToNonAlphaNum();
    } while (this->anyOf(wordStart, prefixes, SK_ARRAY_COUNT(prefixes)));
    if ('*' == this->peek()) {
        this->next();
    }
    return this->className(MarkType::kMember);
}

string BmhParser::methodName() {
    if (this->hasEndToken()) {
        if (!fParent || !fParent->fName.length()) {
            return this->reportError<string>("missing parent method name");
        }
        SkASSERT(fMC == this->peek());
        this->next();
        SkASSERT(fMC == this->peek());
        this->next();
        SkASSERT(fMC != this->peek());
        return fParent->fName;
    }
    string builder;
    const char* end = this->lineEnd();
    const char* paren = this->strnchr('(', end);
    if (!paren) {
        return this->reportError<string>("missing method name and reference");
    }
    const char* nameStart = paren;
    char ch;
    bool expectOperator = false;
    bool isConstructor = false;
    const char* nameEnd = nullptr;
    while (nameStart > fChar && ' ' != (ch = *--nameStart)) {
        if (!isalnum(ch) && '_' != ch) {
            if (nameEnd) {
                break;
            }
            expectOperator = true;
            continue;
        }
        if (!nameEnd) {
            nameEnd = nameStart + 1;
        }
    }
    if (!nameEnd) {
         return this->reportError<string>("unexpected method name char");
    }
    if (' ' == nameStart[0]) {
        ++nameStart;
    }
    if (nameEnd <= nameStart) {
        return this->reportError<string>("missing method name");
    }
    if (nameStart >= paren) {
        return this->reportError<string>("missing method name length");
    }
    string name(nameStart, nameEnd - nameStart);
    bool allLower = true;
    for (int index = 0; index < (int) (nameEnd - nameStart); ++index) {
        if (!islower(nameStart[index])) {
            allLower = false;
            break;
        }
    }
    if (expectOperator && "operator" != name) {
         return this->reportError<string>("expected operator");
    }
    const Definition* parent = this->parentSpace();
    if (parent && parent->fName.length() > 0) {
        if (parent->fName == name) {
            isConstructor = true;
        } else if ('~' == name[0]) {
            if (parent->fName != name.substr(1)) {
                 return this->reportError<string>("expected destructor");
            }
            isConstructor = true;
        }
        builder = parent->fName + "::";
    }
    bool addConst = false;
    if (isConstructor || expectOperator) {
        paren = this->strnchr(')', end) + 1;
        TextParser::Save saveState(this);
        this->skipTo(paren);
        if (this->skipExact("_const")) {
            addConst = true;
        }
        saveState.restore();
    }
    builder.append(nameStart, paren - nameStart);
    if (addConst) {
        builder.append("_const");
    }
    if (!expectOperator && allLower) {
        builder.append("()");
    }
    int parens = 0;
    while (fChar < end || parens > 0) {
        if ('(' == this->peek()) {
            ++parens;
        } else if (')' == this->peek()) {
            --parens;
        }
        this->next();
    }
    TextParser::Save saveState(this);
    this->skipWhiteSpace();
    if (this->startsWith("const")) {
        this->skipName("const");
    } else {
        saveState.restore();
    }
//    this->next();
    return uniqueRootName(builder, MarkType::kMethod);
}

const Definition* BmhParser::parentSpace() const {
    Definition* parent = nullptr;
    Definition* test = fParent;
    while (test) {
        if (MarkType::kClass == test->fMarkType ||
                MarkType::kEnumClass == test->fMarkType ||
                MarkType::kStruct == test->fMarkType) {
            parent = test;
            break;
        }
        test = test->fParent;
    }
    return parent;
}

bool BmhParser::popParentStack(Definition* definition) {
    if (!fParent) {
        return this->reportError<bool>("missing parent");
    }
    if (definition != fParent) {
        return this->reportError<bool>("definition end is not parent");
    }
    if (!definition->fStart) {
        return this->reportError<bool>("definition missing start");
    }
    if (definition->fContentEnd) {
        return this->reportError<bool>("definition already ended");
    }
    definition->fContentEnd = fLine - 1;
    definition->fTerminator = fChar;
    fParent = definition->fParent;
    if (!fParent || (MarkType::kTopic == fParent->fMarkType && !fParent->fParent)) {
        fRoot = nullptr;
    }
    return true;
}

TextParser::TextParser(const Definition* definition) :
    TextParser(definition->fFileName, definition->fContentStart, definition->fContentEnd,
        definition->fLineCount) {
}

void TextParser::reportError(const char* errorStr) const {
    this->reportWarning(errorStr);
    SkDebugf("");  // convenient place to set a breakpoint
}

void TextParser::reportWarning(const char* errorStr) const {
    TextParser err(fFileName, fLine, fEnd, fLineCount);
    size_t lineLen = this->lineLength();
    ptrdiff_t spaces = fChar - fLine;
    while (spaces > 0 && (size_t) spaces > lineLen) {
        ++err.fLineCount;
        err.fLine += lineLen;
        spaces -= lineLen;
        lineLen = err.lineLength();
    }
    SkDebugf("\n%s(%zd): error: %s\n", fFileName.c_str(), err.fLineCount, errorStr);
    if (0 == lineLen) {
        SkDebugf("[blank line]\n");
    } else {
        while (lineLen > 0 && '\n' == err.fLine[lineLen - 1]) {
            --lineLen;
        }
        SkDebugf("%.*s\n", (int) lineLen, err.fLine);
        SkDebugf("%*s^\n", (int) spaces, "");
    }
}

bool BmhParser::skipNoName() {
    if ('\n' == this->peek()) {
        this->next();
        return true;
    }
    this->skipWhiteSpace();
    if (fMC != this->peek()) {
        return this->reportError<bool>("expected end mark");
    }
    this->next();
    if (fMC != this->peek()) {
        return this->reportError<bool>("expected end mark");
    }
    this->next();
    return true;
}

bool BmhParser::skipToDefinitionEnd(MarkType markType) {
    if (this->eof()) {
        return this->reportError<bool>("missing end");
    }
    const char* start = fLine;
    int startLineCount = fLineCount;
    int stack = 1;
    ptrdiff_t lineLen;
    bool foundEnd = false;
    do {
        lineLen = this->lineLength();
        if (fMC != *fChar++) {
            continue;
        }
        if (fMC == *fChar) {
            continue;
        }
        if (' ' == *fChar) {
            continue;
        }
        MarkType nextType = this->getMarkType(MarkLookup::kAllowUnknown);
        if (markType != nextType) {
            continue;
        }
        bool hasEnd = this->hasEndToken();
        if (hasEnd) {
            if (!--stack) {
                foundEnd = true;
                continue;
            }
        } else {
            ++stack;
        }
    } while ((void) ++fLineCount, (void) (fLine += lineLen), (void) (fChar = fLine),
            !this->eof() && !foundEnd);
    if (foundEnd) {
        return true;
    }
    fLineCount = startLineCount;
    fLine = start;
    fChar = start;
    return this->reportError<bool>("unbalanced stack");
}

vector<string> BmhParser::topicName() {
    vector<string> result;
    this->skipWhiteSpace();
    const char* lineEnd = fLine + this->lineLength();
    const char* nameStart = fChar;
    while (fChar < lineEnd) {
        char ch = this->next();
        SkASSERT(',' != ch);
        if ('\n' == ch) {
            break;
        }
        if (fMC == ch) {
            break;
        }
    }
    if (fChar - 1 > nameStart) {
        string builder(nameStart, fChar - nameStart - 1);
        trim_start_end(builder);
        result.push_back(builder);
    }
    if (fChar < lineEnd && fMC == this->peek()) {
        this->next();
    }
    return result;
}

// typeName parsing rules depend on mark type
vector<string> BmhParser::typeName(MarkType markType, bool* checkEnd) {
    fAnonymous = false;
    fCloned = false;
    vector<string> result;
    string builder;
    if (fParent) {
        builder = fParent->fName;
    }
    switch (markType) {
        case MarkType::kEnum:
            // enums may be nameless
        case MarkType::kConst:
        case MarkType::kEnumClass:
        case MarkType::kClass:
        case MarkType::kStruct:
            // expect name
            builder = this->className(markType);
            break;
        case MarkType::kExample:
            // check to see if one already exists -- if so, number this one
            builder = this->uniqueName(string(), markType);
            this->skipNoName();
            break;
        case MarkType::kCode:
        case MarkType::kDeprecated:
        case MarkType::kDescription:
        case MarkType::kDoxygen:
        case MarkType::kExperimental:
        case MarkType::kExternal:
        case MarkType::kFormula:
        case MarkType::kFunction:
        case MarkType::kLegend:
        case MarkType::kList:
        case MarkType::kNoExample:
        case MarkType::kPrivate:
        case MarkType::kTrack:
            this->skipNoName();
            break;
        case MarkType::kAlias:
        case MarkType::kAnchor:
        case MarkType::kBug:  // fixme: expect number
        case MarkType::kDefine:
        case MarkType::kDefinedBy:
        case MarkType::kError:
        case MarkType::kFile:
        case MarkType::kHeight:
        case MarkType::kImage:
        case MarkType::kLiteral:
        case MarkType::kOutdent:
        case MarkType::kPlatform:
        case MarkType::kReturn:
        case MarkType::kSeeAlso:
        case MarkType::kSubstitute:
        case MarkType::kTime:
        case MarkType::kToDo:
        case MarkType::kVolatile:
        case MarkType::kWidth:
            *checkEnd = false;  // no name, may have text body
            break;
        case MarkType::kStdOut:
            this->skipNoName();
            break;  // unnamed
        case MarkType::kMember:
            builder = this->memberName();
            break;
        case MarkType::kMethod:
            builder = this->methodName();
            break;
        case MarkType::kTypedef:
            builder = this->typedefName();
            break;
        case MarkType::kParam:
           // fixme: expect camelCase
            builder = this->word("", "");
            this->skipSpace();
            *checkEnd = false;
            break;
        case MarkType::kTable:
            this->skipNoName();
            break;  // unnamed
        case MarkType::kSubtopic:
        case MarkType::kTopic:
            // fixme: start with cap, allow space, hyphen, stop on comma
            // one topic can have multiple type names delineated by comma
            result = this->topicName();
            if (result.size() == 0 && this->hasEndToken()) {
                break;
            }
            return result;
        default:
            // fixme: don't allow silent failures
            SkASSERT(0);
    }
    result.push_back(builder);
    return result;
}

string BmhParser::typedefName() {
    if (this->hasEndToken()) {
        if (!fParent || !fParent->fName.length()) {
            return this->reportError<string>("missing parent typedef name");
        }
        SkASSERT(fMC == this->peek());
        this->next();
        SkASSERT(fMC == this->peek());
        this->next();
        SkASSERT(fMC != this->peek());
        return fParent->fName;
    }
    // look for typedef as one of three forms:
    // typedef return-type (*NAME)(params);
    // typedef alias NAME;
    // typedef std::function<alias> NAME;
    string builder;
    const char* end = this->doubleLF();
    if (!end) {
       end = fEnd;
    }
    const char* altEnd = this->strnstr("#Typedef ##", end);
    if (altEnd) {
        end = this->strnchr('\n', end);
    }
    if (!end) {
        return this->reportError<string>("missing typedef std::function end bracket >");
    }

    if (this->startsWith("std::function")) {
        if (!this->skipToEndBracket('>')) {
            return this->reportError<string>("missing typedef std::function end bracket >");
        }
        this->next();
        this->skipWhiteSpace();
        builder = string(fChar, end - fChar);
    } else {
        const char* paren = this->strnchr('(', end);
        if (!paren) {
            const char* lastWord = nullptr;
            do {
                this->skipToWhiteSpace();
                if (fChar < end && isspace(fChar[0])) {
                    this->skipWhiteSpace();
                    lastWord = fChar;
                } else {
                    break;
                }
            } while (true);
            if (!lastWord) {
                return this->reportError<string>("missing typedef name");
            }
            builder = string(lastWord, end - lastWord);
        } else {
            this->skipTo(paren);
            this->next();
            if ('*' != this->next()) {
                return this->reportError<string>("missing typedef function asterisk");
            }
            const char* nameStart = fChar;
            if (!this->skipToEndBracket(')')) {
                return this->reportError<string>("missing typedef function )");
            }
            builder = string(nameStart, fChar - nameStart);
            if (!this->skipToEndBracket('(')) {
                return this->reportError<string>("missing typedef params (");
            }
            if (! this->skipToEndBracket(')')) {
                return this->reportError<string>("missing typedef params )");
            }
            this->skipTo(end);
        }
    }
    return uniqueRootName(builder, MarkType::kTypedef);
}

string BmhParser::uniqueName(const string& base, MarkType markType) {
    string builder(base);
    if (!builder.length()) {
        builder = fParent->fName;
    }
    if (!fParent) {
        return builder;
    }
    int number = 2;
    string numBuilder(builder);
    do {
        for (const auto& iter : fParent->fChildren) {
            if (markType == iter->fMarkType) {
                if (iter->fName == numBuilder) {
                    fCloned = true;
                    numBuilder = builder + '_' + to_string(number);
                    goto tryNext;
                }
            }
        }
        break;
tryNext: ;
    } while (++number);
    return numBuilder;
}

string BmhParser::uniqueRootName(const string& base, MarkType markType) {
    auto checkName = [markType](const Definition& def, const string& numBuilder) -> bool {
        return markType == def.fMarkType && def.fName == numBuilder;
    };

        if (string::npos != base.find("SkMatrix::operator")) {
            SkDebugf("");
        }
    string builder(base);
    if (!builder.length()) {
        builder = fParent->fName;
    }
    int number = 2;
    string numBuilder(builder);
    Definition* cloned = nullptr;
    do {
        if (fRoot) {
            for (auto& iter : fRoot->fBranches) {
                if (checkName(*iter.second, numBuilder)) {
                    cloned = iter.second;
                    goto tryNext;
                }
            }
            for (auto& iter : fRoot->fLeaves) {
                if (checkName(iter.second, numBuilder)) {
                    cloned = &iter.second;
                    goto tryNext;
                }
            }
        } else if (fParent) {
            for (auto& iter : fParent->fChildren) {
                if (checkName(*iter, numBuilder)) {
                    cloned = &*iter;
                    goto tryNext;
                }
            }
        }
        break;
tryNext: ;
        if ("()" == builder.substr(builder.length() - 2)) {
            builder = builder.substr(0, builder.length() - 2);
        }
        if (MarkType::kMethod == markType) {
            cloned->fCloned = true;
        }
        fCloned = true;
        if (string::npos != builder.find("operator")) {
            SkDebugf("");
        }
        numBuilder = builder + '_' + to_string(number);
    } while (++number);
    return numBuilder;
}

void BmhParser::validate() const {
    for (int index = 0; index <= (int) Last_MarkType; ++index) {
        SkASSERT(fMaps[index].fMarkType == (MarkType) index);
    }
    const char* last = "";
    for (int index = 0; index <= (int) Last_MarkType; ++index) {
        const char* next = fMaps[index].fName;
        if (!last[0]) {
            last = next;
            continue;
        }
        if (!next[0]) {
            continue;
        }
        SkASSERT(strcmp(last, next) < 0);
        last = next;
    }
}

string BmhParser::word(const string& prefix, const string& delimiter) {
    string builder(prefix);
    this->skipWhiteSpace();
    const char* lineEnd = fLine + this->lineLength();
    const char* nameStart = fChar;
    while (fChar < lineEnd) {
        char ch = this->next();
        if (' ' >= ch) {
            break;
        }
        if (',' == ch) {
            return this->reportError<string>("no comma needed");
            break;
        }
        if (fMC == ch) {
            return builder;
        }
        if (!isalnum(ch) && '_' != ch && ':' != ch && '-' != ch) {
            return this->reportError<string>("unexpected char");
        }
        if (':' == ch) {
            // expect pair, and expect word to start with Sk
            if (nameStart[0] != 'S' || nameStart[1] != 'k') {
                return this->reportError<string>("expected Sk");
            }
            if (':' != this->peek()) {
                return this->reportError<string>("expected ::");
            }
            this->next();
        } else if ('-' == ch) {
            // expect word not to start with Sk or kX where X is A-Z
            if (nameStart[0] == 'k' && nameStart[1] >= 'A' && nameStart[1] <= 'Z') {
                return this->reportError<string>("didn't expected kX");
            }
            if (nameStart[0] == 'S' && nameStart[1] == 'k') {
                return this->reportError<string>("expected Sk");
            }
        }
    }
    if (prefix.size()) {
        builder += delimiter;
    }
    builder.append(nameStart, fChar - nameStart - 1);
    return builder;
}

// pass one: parse text, collect definitions
// pass two: lookup references

static int count_children(const Definition& def, MarkType markType) {
    int count = 0;
    if (markType == def.fMarkType) {
        ++count;
    }
    for (auto& child : def.fChildren ) {
        count += count_children(*child, markType);
    }
    return count;
}

int main(int argc, char** const argv) {
    BmhParser bmhParser(FLAGS_skip);
    bmhParser.validate();

    SkCommandLineFlags::SetUsage(
        "Common Usage: bookmaker -b path/to/bmh_files -i path/to/include.h -t\n"
        "              bookmaker -b path/to/bmh_files -e fiddle.json\n"
        "              ~/go/bin/fiddlecli --input fiddle.json --output fiddleout.json\n"
        "              bookmaker -b path/to/bmh_files -f fiddleout.json -r path/to/md_files\n"
        "              bookmaker -b path/to/bmh_files -i path/to/include.h -x\n"
        "              bookmaker -b path/to/bmh_files -i path/to/include.h -p\n");
    bool help = false;
    for (int i = 1; i < argc; i++) {
        if (0 == strcmp("-h", argv[i]) || 0 == strcmp("--help", argv[i])) {
            help = true;
            for (int j = i + 1; j < argc; j++) {
                if (SkStrStartsWith(argv[j], '-')) {
                    break;
                }
                help = false;
            }
            break;
        }
    }
    if (!help) {
        SkCommandLineFlags::Parse(argc, argv);
    } else {
        SkCommandLineFlags::PrintUsage();
        const char* const commands[] = { "", "-h", "bmh", "-h", "examples", "-h", "include", "-h", "fiddle",
            "-h", "ref", "-h", "tokens",
            "-h", "crosscheck", "-h", "populate", "-h", "spellcheck" };
        SkCommandLineFlags::Parse(SK_ARRAY_COUNT(commands), commands);
        return 0;
    }
    if (FLAGS_bmh.isEmpty() && FLAGS_include.isEmpty()) {
        SkDebugf("requires -b or -i\n");
        SkCommandLineFlags::PrintUsage();
        return 1;
    }
    if ((FLAGS_bmh.isEmpty() || FLAGS_fiddle.isEmpty() || FLAGS_ref.isEmpty()) && FLAGS_catalog) {
        SkDebugf("-c requires -b -f -r\n");
        SkCommandLineFlags::PrintUsage();
        return 1;
    }
    if (FLAGS_bmh.isEmpty() && !FLAGS_examples.isEmpty()) {
        SkDebugf("-e requires -b\n");
        SkCommandLineFlags::PrintUsage();
        return 1;
    }
    if (FLAGS_hack) {
        if (FLAGS_bmh.isEmpty()) {
            SkDebugf("-k or --hack requires -b\n");
            SkCommandLineFlags::PrintUsage();
            return 1;
        }
        HackParser hacker;
        if (!hacker.parseFile(FLAGS_bmh[0], ".bmh")) {
            SkDebugf("hack failed\n");
            return -1;
        }
        return 0;
    }
    if ((FLAGS_include.isEmpty() || FLAGS_bmh.isEmpty()) && FLAGS_populate) {
        SkDebugf("-p requires -b -i\n");
        SkCommandLineFlags::PrintUsage();
        return 1;
    }
    if (FLAGS_bmh.isEmpty() && !FLAGS_ref.isEmpty()) {
        SkDebugf("-r requires -b\n");
        SkCommandLineFlags::PrintUsage();
        return 1;
    }
    if (FLAGS_bmh.isEmpty() && !FLAGS_spellcheck.isEmpty()) {
        SkDebugf("-s requires -b\n");
        SkCommandLineFlags::PrintUsage();
        return 1;
    }
    if ((FLAGS_include.isEmpty() || FLAGS_bmh.isEmpty()) && FLAGS_tokens) {
        SkDebugf("-t requires -b -i\n");
        SkCommandLineFlags::PrintUsage();
        return 1;
    }
    if ((FLAGS_include.isEmpty() || FLAGS_bmh.isEmpty()) && FLAGS_crosscheck) {
        SkDebugf("-x requires -b -i\n");
        SkCommandLineFlags::PrintUsage();
        return 1;
    }
    if (!FLAGS_bmh.isEmpty()) {
        bmhParser.reset();
        if (!bmhParser.parseFile(FLAGS_bmh[0], ".bmh")) {
            return -1;
        }
    }
    bool done = false;
    if (!FLAGS_include.isEmpty()) {
        if (FLAGS_tokens || FLAGS_crosscheck) {
            IncludeParser includeParser;
            includeParser.validate();
            if (!includeParser.parseFile(FLAGS_include[0], ".h")) {
                return -1;
            }
            if (FLAGS_tokens) {
                includeParser.fDebugOut = FLAGS_stdout;
                if (includeParser.dumpTokens(FLAGS_bmh[0])) {
                    bmhParser.fWroteOut = true;
                }
                done = true;
            } else if (FLAGS_crosscheck) {
                if (!includeParser.crossCheck(bmhParser)) {
                    return -1;
                }
                done = true;
            }
        } else if (FLAGS_populate) {
            IncludeWriter includeWriter;
            includeWriter.validate();
            if (!includeWriter.parseFile(FLAGS_include[0], ".h")) {
                return -1;
            }
            includeWriter.fDebugOut = FLAGS_stdout;
            if (!includeWriter.populate(bmhParser)) {
                return -1;
            }
            bmhParser.fWroteOut = true;
            done = true;
        }
    }
    if (!done && !FLAGS_catalog && !FLAGS_fiddle.isEmpty() && FLAGS_examples.isEmpty()) {
        FiddleParser fparser(&bmhParser);
        if (!fparser.parseFile(FLAGS_fiddle[0], ".txt")) {
            return -1;
        }
    }
    if (!done && FLAGS_catalog && FLAGS_examples.isEmpty()) {
        Catalog fparser(&bmhParser);
        fparser.fDebugOut = FLAGS_stdout;
        if (!fparser.openCatalog(FLAGS_bmh[0], FLAGS_ref[0])) {
            return -1;
        }
        if (!fparser.parseFile(FLAGS_fiddle[0], ".txt")) {
            return -1;
        }
        if (!fparser.closeCatalog()) {
            return -1;
        }
        bmhParser.fWroteOut = true;
        done = true;
    }
    if (!done && !FLAGS_ref.isEmpty() && FLAGS_examples.isEmpty()) {
        MdOut mdOut(bmhParser);
        mdOut.fDebugOut = FLAGS_stdout;
        if (mdOut.buildReferences(FLAGS_bmh[0], FLAGS_ref[0])) {
            bmhParser.fWroteOut = true;
        }
    }
    if (!done && !FLAGS_spellcheck.isEmpty() && FLAGS_examples.isEmpty()) {
        bmhParser.spellCheck(FLAGS_bmh[0], FLAGS_spellcheck);
        bmhParser.fWroteOut = true;
        done = true;
    }
    int examples = 0;
    int methods = 0;
    int topics = 0;
    if (!done && !FLAGS_examples.isEmpty()) {
        // check to see if examples have duplicate names
        if (!bmhParser.checkExamples()) {
            return -1;
        }
        bmhParser.fDebugOut = FLAGS_stdout;
        if (!bmhParser.dumpExamples(FLAGS_examples[0])) {
            return -1;
        }
        return 0;
    }
    if (!bmhParser.fWroteOut) {
        for (const auto& topic : bmhParser.fTopicMap) {
            if (topic.second->fParent) {
                continue;
            }
            examples += count_children(*topic.second, MarkType::kExample);
            methods += count_children(*topic.second, MarkType::kMethod);
            topics += count_children(*topic.second, MarkType::kSubtopic);
            topics += count_children(*topic.second, MarkType::kTopic);
        }
        SkDebugf("topics=%d classes=%d methods=%d examples=%d\n",
                bmhParser.fTopicMap.size(), bmhParser.fClassMap.size(),
                methods, examples);
    }
    return 0;
}
