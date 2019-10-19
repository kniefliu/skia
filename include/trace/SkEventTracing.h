/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SkEventTracing_DEFINED
#define SkEventTracing_DEFINED

/**
 * Construct and install an SkEventTracer, based on the mode,
 * defaulting to the --trace command line argument.
 */
class SK_API SkEventTracing {
public:
    static void initializeEventTracingForTools(const char* traceFileName);
};

#endif
