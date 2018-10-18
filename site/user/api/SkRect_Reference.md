SkRect Reference
===

# <a name="Rect"></a> Rect

# <a name="SkRect"></a> Struct SkRect
<a href="#SkRect">SkRect</a> holds four <a href="undocumented#SkScalar">SkScalar</a> coordinates describing the upper and
lower bounds of a rectangle. <a href="#SkRect">SkRect</a> may be created from outer bounds or 
from position, <a href="#SkRect_width">width</a>, and <a href="#SkRect_height">height</a>. <a href="#SkRect">SkRect</a> describes an area; if its <a href="#SkRect_right">right</a>
is less than or equal to its <a href="#SkRect_left">left</a>, or if its <a href="#SkRect_bottom">bottom</a> is less than or equal to
its <a href="#SkRect_top">top</a>, it is considered empty.

<a href="#SkRect">SkRect</a> can be constructed from int values to avoid compiler warnings that
integer input cannot convert to <a href="undocumented#SkScalar">SkScalar</a> without loss of precision.

# <a name="Overview"></a> Overview

## <a name="Subtopics"></a> Subtopics

| topics | description |
| --- | ---  |

## <a name="Operators"></a> Operators

| description | function |
| --- | ---  |
| bool <a href="#SkRect_notequal_operator">operator!=(const SkRect& a, const SkRect& b)</a> | Returns true if members are unequal. |
| bool <a href="#SkRect_equal_operator">operator==(const SkRect& a, const SkRect& b)</a> | Returns true if members are equal. |

## <a name="Member_Functions"></a> Member Functions

| description | function |
| --- | ---  |
| <a href="#SkRect_Intersects">Intersects</a> | Returns true if areas overlap. |
| <a href="#SkRect_Make">Make</a> | Constructs from <a href="undocumented#ISize">ISize</a> returning (0, 0, <a href="#SkRect_width">width</a>, <a href="#SkRect_height">height</a>). |
| <a href="#SkRect_MakeEmpty">MakeEmpty</a> | Constructs from bounds of (0, 0, 0, 0). |
| <a href="#SkRect_MakeFromIRect">MakeFromIRect</a> | Deprecated. |
| <a href="#SkRect_MakeIWH">MakeIWH</a> | Constructs from int input returning (0, 0, <a href="#SkRect_width">width</a>, <a href="#SkRect_height">height</a>). |
| <a href="#SkRect_MakeLTRB">MakeLTRB</a> | Constructs from <a href="undocumented#SkScalar">SkScalar</a> <a href="#SkRect_left">left</a>, <a href="#SkRect_top">top</a>, <a href="#SkRect_right">right</a>, <a href="#SkRect_bottom">bottom</a>. |
| <a href="#SkRect_MakeLargest">MakeLargest</a> | Constructs (<a href="undocumented#SK_ScalarMin">SK ScalarMin</a>, <a href="undocumented#SK_ScalarMin">SK ScalarMin</a>, <a href="undocumented#SK_ScalarMax">SK ScalarMax</a>, <a href="undocumented#SK_ScalarMax">SK ScalarMax</a>). |
| <a href="#SkRect_MakeLargestS32">MakeLargestS32</a> | Constructs largest signed integers that fit in 32-bit float. |
| <a href="#SkRect_MakeSize">MakeSize</a> | Constructs from <a href="undocumented#Size">Size</a> returning (0, 0, <a href="#SkRect_width">width</a>, <a href="#SkRect_height">height</a>). |
| <a href="#SkRect_MakeWH">MakeWH</a> | Constructs from <a href="undocumented#SkScalar">SkScalar</a> input returning (0, 0, <a href="#SkRect_width">width</a>, <a href="#SkRect_height">height</a>). |
| <a href="#SkRect_MakeXYWH">MakeXYWH</a> | Constructs from <a href="undocumented#SkScalar">SkScalar</a> input returning (<a href="#SkRect_x">x</a>, <a href="#SkRect_y">y</a>, <a href="#SkRect_width">width</a>, <a href="#SkRect_height">height</a>). |
| <a href="#SkRect_asScalars">asScalars</a> | Returns pointer to members as array. |
| <a href="#SkRect_bottom">bottom</a> | Returns larger bounds in <a href="#SkRect_y">y</a>, if sorted. |
| <a href="#SkRect_centerX">centerX</a> | Returns midpoint in <a href="#SkRect_x">x</a>. |
| <a href="#SkRect_centerY">centerY</a> | Returns midpoint in <a href="#SkRect_y">y</a>. |
| <a href="#SkRect_contains">contains</a> | Returns true if points are equal or inside. |
| <a href="#SkRect_dump_2">dump</a> | Sends text representation using floats to standard output. |
| <a href="#SkRect_dumpHex">dumpHex</a> | Sends text representation using hexadecimal to standard output. |
| <a href="#SkRect_growToInclude">growToInclude</a> | Sets to union of bounds and one or more <a href="#Point">Points</a>. |
| <a href="#SkRect_height">height</a> | Returns span in <a href="#SkRect_y">y</a>. |
| <a href="#SkRect_inset">inset</a> | Moves the sides symmetrically about the center. |
| <a href="#SkRect_intersect">intersect</a> | Sets to shared area; returns true if not empty. |
| <a href="#SkRect_intersects">intersects</a> | Returns true if areas overlap. |
| <a href="#SkRect_isEmpty">isEmpty</a> | Returns true if <a href="#SkRect_width">width</a> or <a href="#SkRect_height">height</a> are zero or negative. |
| <a href="#SkRect_isFinite">isFinite</a> | Returns true if no member is infinite or <a href="undocumented#NaN">NaN</a>. |
| <a href="#SkRect_isLargest">isLargest</a> | Returns equal to (<a href="undocumented#SK_ScalarMin">SK ScalarMin</a>, <a href="undocumented#SK_ScalarMin">SK ScalarMin</a>, <a href="undocumented#SK_ScalarMax">SK ScalarMax</a>, <a href="undocumented#SK_ScalarMax">SK ScalarMax</a>). |
| <a href="#SkRect_isSorted">isSorted</a> | Returns true if <a href="#SkRect_width">width</a> or <a href="#SkRect_height">height</a> are zero or positive. |
| <a href="#SkRect_iset">iset</a> | Sets to int input (<a href="#SkRect_left">left</a>, <a href="#SkRect_top">top</a>, <a href="#SkRect_right">right</a>, <a href="#SkRect_bottom">bottom</a>). |
| <a href="#SkRect_isetWH">isetWH</a> | Sets to int input (0, 0, <a href="#SkRect_width">width</a>, <a href="#SkRect_height">height</a>). |
| <a href="#SkRect_join">join</a> | Sets to union of bounds. |
| <a href="#SkRect_joinNonEmptyArg">joinNonEmptyArg</a> | Sets to union of bounds, asserting that argument is not empty. |
| <a href="#SkRect_joinPossiblyEmptyRect">joinPossiblyEmptyRect</a> | Sets to union of bounds. Skips empty check for both. |
| <a href="#SkRect_left">left</a> | Returns smaller bounds in <a href="#SkRect_x">x</a>, if sorted. |
| <a href="#SkRect_makeInset">makeInset</a> | Constructs from sides moved symmetrically about the center. |
| <a href="#SkRect_makeOffset">makeOffset</a> | Constructs from translated sides. |
| <a href="#SkRect_makeOutset">makeOutset</a> | Constructs from sides moved symmetrically about the center. |
| <a href="#SkRect_makeSorted">makeSorted</a> | Constructs, ordering sides from smaller to larger. |
| <a href="#SkRect_offset">offset</a> | Translates sides without changing <a href="#SkRect_width">width</a> and <a href="#SkRect_height">height</a>. |
| <a href="#SkRect_offsetTo">offsetTo</a> | Translates to (<a href="#SkRect_x">x</a>, <a href="#SkRect_y">y</a>) without changing <a href="#SkRect_width">width</a> and <a href="#SkRect_height">height</a>. |
| <a href="#SkRect_outset">outset</a> | Moves the sides symmetrically about the center. |
| <a href="#SkRect_right">right</a> | Returns larger bounds in <a href="#SkRect_x">x</a>, if sorted. |
| <a href="#SkRect_round_2">round</a> | Sets members to nearest integer value. |
| <a href="#SkRect_roundIn">roundIn</a> | Sets members to nearest integer value towards opposite. |
| <a href="#SkRect_roundOut">roundOut</a> | Sets members to nearest integer value away from opposite. |
| <a href="#SkRect_set">set</a> | Sets to <a href="undocumented#SkScalar">SkScalar</a> input (<a href="#SkRect_left">left</a>, <a href="#SkRect_top">top</a>, <a href="#SkRect_right">right</a>, <a href="#SkRect_bottom">bottom</a>) and others. |
| <a href="#SkRect_setBounds">setBounds</a> | Sets to upper and lower limits of <a href="SkPoint_Reference#Point">Point</a> array. |
| <a href="#SkRect_setBoundsCheck">setBoundsCheck</a> | Sets to upper and lower limits of <a href="SkPoint_Reference#Point">Point</a> array. |
| <a href="#SkRect_setEmpty">setEmpty</a> | Sets to (0, 0, 0, 0). |
| <a href="#SkRect_setLTRB">setLTRB</a> | Sets to <a href="undocumented#SkScalar">SkScalar</a> input (<a href="#SkRect_left">left</a>, <a href="#SkRect_top">top</a>, <a href="#SkRect_right">right</a>, <a href="#SkRect_bottom">bottom</a>). |
| <a href="#SkRect_setLargest">setLargest</a> | Sets to (<a href="undocumented#SK_ScalarMin">SK ScalarMin</a>, <a href="undocumented#SK_ScalarMin">SK ScalarMin</a>, <a href="undocumented#SK_ScalarMax">SK ScalarMax</a>, <a href="undocumented#SK_ScalarMax">SK ScalarMax</a>). |
| <a href="#SkRect_setLargestInverted">setLargestInverted</a> | Sets to (<a href="undocumented#SK_ScalarMax">SK ScalarMax</a>, <a href="undocumented#SK_ScalarMax">SK ScalarMax</a>, <a href="undocumented#SK_ScalarMin">SK ScalarMin</a>, <a href="undocumented#SK_ScalarMin">SK ScalarMin</a>). |
| <a href="#SkRect_setWH">setWH</a> | Sets to <a href="undocumented#SkScalar">SkScalar</a> input (0, 0, <a href="#SkRect_width">width</a>, <a href="#SkRect_height">height</a>). |
| <a href="#SkRect_setXYWH">setXYWH</a> | Sets to <a href="undocumented#SkScalar">SkScalar</a> input (<a href="#SkRect_x">x</a>, <a href="#SkRect_y">y</a>, <a href="#SkRect_width">width</a>, <a href="#SkRect_height">height</a>). |
| <a href="#SkRect_sort">sort</a> | Orders sides from smaller to larger. |
| <a href="#SkRect_toQuad">toQuad</a> | Returns four corners as <a href="SkPoint_Reference#Point">Point</a>. |
| <a href="#SkRect_top">top</a> | Returns smaller bounds in <a href="#SkRect_y">y</a>, if sorted. |
| <a href="#SkRect_width">width</a> | Returns span in <a href="#SkRect_x">x</a>. |
| <a href="#SkRect_x">x</a> | Returns bounds <a href="#SkRect_left">left</a>. |
| <a href="#SkRect_y">y</a> | Returns bounds <a href="#SkRect_top">top</a>. |

<a name="SkRect_fLeft"> <code><strong>SkScalar  fLeft</strong></code> </a>

May contain any value, including infinities and <a href="undocumented#NaN">NaN</a>. The smaller of the
horizontal values when sorted. When equal to or greater than <a href="#SkRect_fRight">fRight</a>, <a href="#Rect">Rect</a> is empty.

<a name="SkRect_fTop"> <code><strong>SkScalar  fTop</strong></code> </a>

May contain any value, including infinities and <a href="undocumented#NaN">NaN</a>. The smaller of the
vertical values when sorted. When equal to or greater than <a href="#SkRect_fBottom">fBottom</a>, <a href="#Rect">Rect</a> is empty.

<a name="SkRect_fRight"> <code><strong>SkScalar  fRight</strong></code> </a>

May contain any value, including infinities and <a href="undocumented#NaN">NaN</a>. The larger of the
horizontal values when sorted. When equal to or less than <a href="#SkRect_fLeft">fLeft</a>, <a href="#Rect">Rect</a> is empty.

<a name="SkRect_fBottom"> <code><strong>SkScalar  fBottom</strong></code> </a>

May contain any value, including infinities and <a href="undocumented#NaN">NaN</a>. The larger of the
vertical values when sorted. When equal to or less than <a href="#SkRect_fTop">fTop</a>, <a href="#Rect">Rect</a> is empty.

<a name="SkRect_MakeEmpty"></a>
## MakeEmpty

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
static constexpr SkRect SK_WARN_UNUSED_RESULT MakeEmpty()
</pre>

Returns constructed <a href="#Rect">Rect</a> <a href="#SkRect_set">set</a> to (0, 0, 0, 0).
Many other rectangles are empty; if <a href="#SkRect_left">left</a> is equal to or greater than <a href="#SkRect_right">right</a>,
or if <a href="#SkRect_top">top</a> is equal to or greater than <a href="#SkRect_bottom">bottom</a>. Setting all members to zero
is a convenience, but does not designate a special empty rectangle.

### Return Value

bounds (0, 0, 0, 0)

### Example

<div><fiddle-embed name="2e262d0ac4b8ef51695e0525fc3ecdf6">

#### Example Output

~~~~
MakeEmpty isEmpty: true
offset rect isEmpty: true
inset rect isEmpty: true
outset rect isEmpty: false
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_isEmpty">isEmpty</a> <a href="#SkRect_setEmpty">setEmpty</a> <a href="#SkRect_setLargestInverted">setLargestInverted</a> <a href="#SkIRect_MakeEmpty">SkIRect::MakeEmpty</a>

---

<a name="SkRect_MakeLargest"></a>
## MakeLargest

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
static SkRect SK_WARN_UNUSED_RESULT MakeLargest()
</pre>

Returns constructed <a href="#Rect">Rect</a> setting <a href="#SkRect_left">left</a> and <a href="#SkRect_top">top</a> to most negative finite value, and 
setting <a href="#SkRect_right">right</a> and <a href="#SkRect_bottom">bottom</a> to most positive finite value.

### Return Value

bounds (<a href="undocumented#SK_ScalarMin">SK ScalarMin</a>, <a href="undocumented#SK_ScalarMin">SK ScalarMin</a>, <a href="undocumented#SK_ScalarMax">SK ScalarMax</a>, <a href="undocumented#SK_ScalarMax">SK ScalarMax</a>)

### Example

<div><fiddle-embed name="42774381948ce88ec19b5c0d16a84bd7">

#### Example Output

~~~~
MakeLargest isLargest: true
MakeLargest isFinite: true
outset a little isLargest: true
outset a little isFinite: true
outset a little more isLargest: false
outset a little more isFinite: false
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_MakeLargestS32">MakeLargestS32</a> <a href="#SkRect_isLargest">isLargest</a> <a href="#SkRect_setLargest">setLargest</a> <a href="#SkIRect_MakeLargest">SkIRect::MakeLargest</a>

---

<a name="SkRect_MakeLargestS32"></a>
## MakeLargestS32

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
static SkRect SK_WARN_UNUSED_RESULT MakeLargestS32()
</pre>

Returns constructed <a href="#Rect">Rect</a> that can be represented exactly with <a href="SkIRect_Reference#IRect">IRect</a>. The <a href="#SkRect_left">left</a>
and <a href="#SkRect_top">top</a> are <a href="#SkRect_set">set</a> to the most negative integer value that fits in a 32-bit float,
and the <a href="#SkRect_right">right</a> and <a href="#SkRect_bottom">bottom</a> are <a href="#SkRect_set">set</a> to the most positive finite value that fits in
a 32-bit float.

These are the largest values for which <a href="#SkRect_round_2">round</a> is well defined.

### Return Value

bounds (<a href="undocumented#SK_MinS32FitsInFloat">SK MinS32FitsInFloat</a>, <a href="undocumented#SK_MinS32FitsInFloat">SK MinS32FitsInFloat</a>,
<a href="undocumented#SK_MaxS32FitsInFloat">SK MaxS32FitsInFloat</a>, <a href="undocumented#SK_MaxS32FitsInFloat">SK MaxS32FitsInFloat</a>)

### Example

<div><fiddle-embed name="9dde55347ba15b60166b5a0583c55587">

#### Example Output

~~~~
f_rect == r_rect
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_MakeLargest">MakeLargest</a> <a href="#SkRect_isLargest">isLargest</a> <a href="#SkRect_setLargest">setLargest</a> <a href="#SkIRect_MakeLargest">SkIRect::MakeLargest</a>

---

<a name="SkRect_MakeWH"></a>
## MakeWH

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
static constexpr SkRect SK_WARN_UNUSED_RESULT MakeWH(SkScalar w, SkScalar h)
</pre>

Returns constructed <a href="#Rect">Rect</a> <a href="#SkRect_set">set</a> to <a href="undocumented#SkScalar">SkScalar</a> values (0, 0, <a href="#SkRect_MakeWH_w">w</a>, <a href="#SkRect_MakeWH_h">h</a>). Does not
validate input; <a href="#SkRect_MakeWH_w">w</a> or <a href="#SkRect_MakeWH_h">h</a> may be negative.

Passing integer values may generate a compiler warning since <a href="#Rect">Rect</a> cannot
represent 32-bit integers exactly. Use <a href="SkIRect_Reference#SkIRect">SkIRect</a> for an exact integer rectangle.

### Parameters

<table>  <tr>    <td><a name="SkRect_MakeWH_w"> <code><strong>w </strong></code> </a></td> <td>
<a href="undocumented#SkScalar">SkScalar</a> <a href="#SkRect_width">width</a> of constructed <a href="#Rect">Rect</a></td>
  </tr>  <tr>    <td><a name="SkRect_MakeWH_h"> <code><strong>h </strong></code> </a></td> <td>
<a href="undocumented#SkScalar">SkScalar</a> <a href="#SkRect_height">height</a> of constructed <a href="#Rect">Rect</a></td>
  </tr>
</table>

### Return Value

bounds (0, 0, <a href="#SkRect_MakeWH_w">w</a>, <a href="#SkRect_MakeWH_h">h</a>)

### Example

<div><fiddle-embed name="8009d30f431e01f8aea4808e9017d9bf">

#### Example Output

~~~~
all equal
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_MakeSize">MakeSize</a> <a href="#SkRect_MakeXYWH">MakeXYWH</a> <a href="#SkRect_MakeIWH">MakeIWH</a> <a href="#SkRect_setWH">setWH</a> <a href="#SkIRect_MakeWH">SkIRect::MakeWH</a>

---

<a name="SkRect_MakeIWH"></a>
## MakeIWH

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
static SkRect SK_WARN_UNUSED_RESULT MakeIWH(int w, int h)
</pre>

Returns constructed <a href="#Rect">Rect</a> <a href="#SkRect_set">set</a> to integer values (0, 0, <a href="#SkRect_MakeIWH_w">w</a>, <a href="#SkRect_MakeIWH_h">h</a>). Does not validate
input; <a href="#SkRect_MakeIWH_w">w</a> or <a href="#SkRect_MakeIWH_h">h</a> may be negative.

Use to avoid a compiler warning that input may lose precision when stored.
Use <a href="SkIRect_Reference#SkIRect">SkIRect</a> for an exact integer rectangle.

### Parameters

<table>  <tr>    <td><a name="SkRect_MakeIWH_w"> <code><strong>w </strong></code> </a></td> <td>
integer <a href="#SkRect_width">width</a> of constructed <a href="#Rect">Rect</a></td>
  </tr>  <tr>    <td><a name="SkRect_MakeIWH_h"> <code><strong>h </strong></code> </a></td> <td>
integer <a href="#SkRect_height">height</a> of constructed <a href="#Rect">Rect</a></td>
  </tr>
</table>

### Return Value

bounds (0, 0, <a href="#SkRect_MakeIWH_w">w</a>, <a href="#SkRect_MakeIWH_h">h</a>)

### Example

<div><fiddle-embed name="faa660ac19eaddc3f3eab57a0bddfdcb">

#### Example Output

~~~~
i_rect width: 25 f_rect width:25
i_rect width: 125000111 f_rect width:125000112
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_MakeXYWH">MakeXYWH</a> <a href="#SkRect_MakeWH">MakeWH</a> <a href="#SkRect_isetWH">isetWH</a> <a href="#SkIRect_MakeWH">SkIRect::MakeWH</a>

---

<a name="SkRect_MakeSize"></a>
## MakeSize

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
static constexpr SkRect SK_WARN_UNUSED_RESULT MakeSize(const SkSize& size)
</pre>

Returns constructed <a href="#Rect">Rect</a> <a href="#SkRect_set">set</a> to (0, 0, <a href="#SkRect_MakeSize_size">size</a>.<a href="#SkRect_width">width</a>, <a href="#SkRect_MakeSize_size">size</a>.<a href="#SkRect_height">height</a>). Does not
validate input; <a href="#SkRect_MakeSize_size">size</a>.<a href="#SkRect_width">width</a> or <a href="#SkRect_MakeSize_size">size</a>.<a href="#SkRect_height">height</a> may be negative.

### Parameters

<table>  <tr>    <td><a name="SkRect_MakeSize_size"> <code><strong>size </strong></code> </a></td> <td>
<a href="undocumented#SkScalar">SkScalar</a> values for <a href="#Rect">Rect</a> <a href="#SkRect_width">width</a> and <a href="#SkRect_height">height</a></td>
  </tr>
</table>

### Return Value

bounds (0, 0, <a href="#SkRect_MakeSize_size">size</a>.<a href="#SkRect_width">width</a>, <a href="#SkRect_MakeSize_size">size</a>.<a href="#SkRect_height">height</a>)

### Example

<div><fiddle-embed name="ab2c1a55016c8de9172b77fdf69e00a2">

#### Example Output

~~~~
rect width: 25.5  height: 35.5
floor width: 25  height: 35
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_MakeWH">MakeWH</a> <a href="#SkRect_MakeXYWH">MakeXYWH</a> <a href="#SkRect_MakeIWH">MakeIWH</a> <a href="#SkRect_setWH">setWH</a> <a href="#SkIRect_MakeWH">SkIRect::MakeWH</a>

---

<a name="SkRect_MakeLTRB"></a>
## MakeLTRB

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
static constexpr SkRect SK_WARN_UNUSED_RESULT MakeLTRB(SkScalar l, SkScalar t, SkScalar r, SkScalar b)
</pre>

Returns constructed <a href="#Rect">Rect</a> <a href="#SkRect_set">set</a> to (<a href="#SkRect_MakeLTRB_l">l</a>, <a href="#SkRect_MakeLTRB_t">t</a>, <a href="#SkRect_MakeLTRB_r">r</a>, <a href="#SkRect_MakeLTRB_b">b</a>). Does not <a href="#SkRect_sort">sort</a> input; <a href="#Rect">Rect</a> may
result in <a href="#SkRect_fLeft">fLeft</a> greater than <a href="#SkRect_fRight">fRight</a>, or <a href="#SkRect_fTop">fTop</a> greater than <a href="#SkRect_fBottom">fBottom</a>.

### Parameters

<table>  <tr>    <td><a name="SkRect_MakeLTRB_l"> <code><strong>l </strong></code> </a></td> <td>
<a href="undocumented#SkScalar">SkScalar</a> stored in <a href="#SkRect_fLeft">fLeft</a></td>
  </tr>  <tr>    <td><a name="SkRect_MakeLTRB_t"> <code><strong>t </strong></code> </a></td> <td>
<a href="undocumented#SkScalar">SkScalar</a> stored in <a href="#SkRect_fTop">fTop</a></td>
  </tr>  <tr>    <td><a name="SkRect_MakeLTRB_r"> <code><strong>r </strong></code> </a></td> <td>
<a href="undocumented#SkScalar">SkScalar</a> stored in <a href="#SkRect_fRight">fRight</a></td>
  </tr>  <tr>    <td><a name="SkRect_MakeLTRB_b"> <code><strong>b </strong></code> </a></td> <td>
<a href="undocumented#SkScalar">SkScalar</a> stored in <a href="#SkRect_fBottom">fBottom</a></td>
  </tr>
</table>

### Return Value

bounds (<a href="#SkRect_MakeLTRB_l">l</a>, <a href="#SkRect_MakeLTRB_t">t</a>, <a href="#SkRect_MakeLTRB_r">r</a>, <a href="#SkRect_MakeLTRB_b">b</a>)

### Example

<div><fiddle-embed name="158b8dd9d02d65a5ae5ab7d1595a5b4c">

#### Example Output

~~~~
rect: 5, 35, 15, 25  isEmpty: true
rect: 5, 25, 15, 35  isEmpty: false
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_MakeXYWH">MakeXYWH</a> <a href="#SkIRect_MakeLTRB">SkIRect::MakeLTRB</a>

---

<a name="SkRect_MakeXYWH"></a>
## MakeXYWH

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
static constexpr SkRect SK_WARN_UNUSED_RESULT MakeXYWH(SkScalar x, SkScalar y, SkScalar w, SkScalar h)
</pre>

Returns constructed <a href="#Rect">Rect</a> <a href="#SkRect_set">set</a> to(<a href="#SkRect_x">x</a>, <a href="#SkRect_y">y</a>, <a href="#SkRect_x">x</a> + <a href="#SkRect_MakeXYWH_w">w</a>, <a href="#SkRect_y">y</a> + <a href="#SkRect_MakeXYWH_h">h</a>). 
Does not validate input;
<a href="#SkRect_MakeXYWH_w">w</a> or <a href="#SkRect_MakeXYWH_h">h</a> may be negative.

### Parameters

<table>  <tr>    <td><a name="SkRect_MakeXYWH_x"> <code><strong>x </strong></code> </a></td> <td>
stored in <a href="#SkRect_fLeft">fLeft</a></td>
  </tr>  <tr>    <td><a name="SkRect_MakeXYWH_y"> <code><strong>y </strong></code> </a></td> <td>
stored in <a href="#SkRect_fTop">fTop</a></td>
  </tr>  <tr>    <td><a name="SkRect_MakeXYWH_w"> <code><strong>w </strong></code> </a></td> <td>
added to <a href="#SkRect_x">x</a> and stored in <a href="#SkRect_fRight">fRight</a></td>
  </tr>  <tr>    <td><a name="SkRect_MakeXYWH_h"> <code><strong>h </strong></code> </a></td> <td>
added to <a href="#SkRect_y">y</a> and stored in <a href="#SkRect_fBottom">fBottom</a></td>
  </tr>
</table>

### Return Value

bounds at (<a href="#SkRect_x">x</a>, <a href="#SkRect_y">y</a>) with <a href="#SkRect_width">width</a> <a href="#SkRect_MakeXYWH_w">w</a> and <a href="#SkRect_height">height</a> <a href="#SkRect_MakeXYWH_h">h</a>

### Example

<div><fiddle-embed name="38e464dba13be11ac21e210fbf3b5afc">

#### Example Output

~~~~
rect: 5, 35, -10, 60  isEmpty: true
rect: -10, 35, 5, 60  isEmpty: false
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_MakeLTRB">MakeLTRB</a> <a href="#SkIRect_MakeXYWH">SkIRect::MakeXYWH</a>

---

<a name="SkRect_MakeFromIRect"></a>
## MakeFromIRect

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
static SkRect SK_WARN_UNUSED_RESULT MakeFromIRect(const SkIRect& irect)
</pre>

Deprecated.

### Parameters

<table>  <tr>    <td><a name="SkRect_MakeFromIRect_irect"> <code><strong>irect </strong></code> </a></td> <td>
integer rect</td>
  </tr>
</table>

### Return Value

<a href="#SkRect_MakeFromIRect_irect">irect</a> as <a href="#SkRect">SkRect</a>

### See Also

<a href="#SkRect_Make">Make</a>

---

<a name="SkRect_Make"></a>
## Make

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
static SkRect Make(const SkISize& size)
</pre>

Returns constructed <a href="SkIRect_Reference#IRect">IRect</a> <a href="#SkRect_set">set</a> to (0, 0, <a href="#SkRect_Make_size">size</a>.<a href="#SkRect_width">width</a>, <a href="#SkRect_Make_size">size</a>.<a href="#SkRect_height">height</a>). 
Does not validate input; <a href="#SkRect_Make_size">size</a>.<a href="#SkRect_width">width</a> or <a href="#SkRect_Make_size">size</a>.<a href="#SkRect_height">height</a> may be negative.

### Parameters

<table>  <tr>    <td><a name="SkRect_Make_size"> <code><strong>size </strong></code> </a></td> <td>
integer values for <a href="#Rect">Rect</a> <a href="#SkRect_width">width</a> and <a href="#SkRect_height">height</a></td>
  </tr>
</table>

### Return Value

bounds (0, 0, <a href="#SkRect_Make_size">size</a>.<a href="#SkRect_width">width</a>, <a href="#SkRect_Make_size">size</a>.<a href="#SkRect_height">height</a>)

### Example

<div><fiddle-embed name="e866f5e4f6ac52e89acadf48e54ac8e0">

#### Example Output

~~~~
rect1 == rect2
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_MakeWH">MakeWH</a> <a href="#SkRect_MakeXYWH">MakeXYWH</a> <a href="#SkRect_MakeIWH">SkRect::MakeIWH</a> <a href="#SkIRect_MakeSize">SkIRect::MakeSize</a>

---

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
static SkRect SK_WARN_UNUSED_RESULT Make(const SkIRect& irect)
</pre>

Returns constructed <a href="SkIRect_Reference#IRect">IRect</a> <a href="#SkRect_set">set</a> to <a href="#SkRect_Make_2_irect">irect</a>, promoting integers to <a href="#Scalar">Scalar</a>.
Does not validate input; <a href="#SkRect_fLeft">fLeft</a> may be greater than <a href="#SkRect_fRight">fRight</a>, <a href="#SkRect_fTop">fTop</a> may be greater
than <a href="#SkRect_fBottom">fBottom</a>.

### Parameters

<table>  <tr>    <td><a name="SkRect_Make_2_irect"> <code><strong>irect </strong></code> </a></td> <td>
integer unsorted bounds</td>
  </tr>
</table>

### Return Value

<a href="#SkRect_Make_2_irect">irect</a> members converted to <a href="undocumented#SkScalar">SkScalar</a>

### Example

<div><fiddle-embed name="dd801faa1e60a0fe9e0657674461e063"></fiddle-embed></div>

### See Also

<a href="#SkRect_MakeLTRB">MakeLTRB</a>

---

<a name="SkRect_isEmpty"></a>
## isEmpty

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
bool isEmpty() const
</pre>

Returns true if <a href="#SkRect_fLeft">fLeft</a> is equal to or greater than <a href="#SkRect_fRight">fRight</a>, or if <a href="#SkRect_fTop">fTop</a> is equal
to or greater than <a href="#SkRect_fBottom">fBottom</a>. Call <a href="#SkRect_sort">sort</a> to reverse rectangles with negative
<a href="#SkRect_width">width</a> or <a href="#SkRect_height">height</a>.

### Return Value

true if <a href="#SkRect_width">width</a> or <a href="#SkRect_height">height</a> are zero or negative

### Example

<div><fiddle-embed name="1d7b924d6ca2a6aef09684a8a632439c">

#### Example Output

~~~~
rect: {20, 40, 10, 50} is empty
sorted: {10, 40, 20, 50} is not empty
rect: {20, 40, 20, 50} is empty
sorted: {20, 40, 20, 50} is empty
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_MakeEmpty">MakeEmpty</a> <a href="#SkRect_sort">sort</a> <a href="#SkIRect_isEmpty">SkIRect::isEmpty</a>

---

<a name="SkRect_isSorted"></a>
## isSorted

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
bool isSorted() const
</pre>

Returns true if <a href="#SkRect_fLeft">fLeft</a> is equal to or less than <a href="#SkRect_fRight">fRight</a>, or if <a href="#SkRect_fTop">fTop</a> is equal
to or less than <a href="#SkRect_fBottom">fBottom</a>. Call <a href="#SkRect_sort">sort</a> to reverse rectangles with negative
<a href="#SkRect_width">width</a> or <a href="#SkRect_height">height</a>.

### Return Value

true if <a href="#SkRect_width">width</a> or <a href="#SkRect_height">height</a> are zero or positive

### Example

<div><fiddle-embed name="c7065a83b220a96f903dbbb65906fe7b">

#### Example Output

~~~~
rect: {20, 40, 10, 50} is not sorted
sorted: {10, 40, 20, 50} is sorted
rect: {20, 40, 20, 50} is sorted
sorted: {20, 40, 20, 50} is sorted
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_sort">sort</a> <a href="#SkRect_makeSorted">makeSorted</a> <a href="#SkRect_isEmpty">isEmpty</a>

---

<a name="SkRect_isLargest"></a>
## isLargest

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
bool isLargest() const
</pre>

Returns true if <a href="#Rect">Rect</a> encloses largest possible area. 

### Return Value

true if equal to (<a href="undocumented#SK_ScalarMin">SK ScalarMin</a>, <a href="undocumented#SK_ScalarMin">SK ScalarMin</a>, <a href="undocumented#SK_ScalarMax">SK ScalarMax</a>, <a href="undocumented#SK_ScalarMax">SK ScalarMax</a>)

### Example

<div><fiddle-embed name="30ef8865ff1ba8789ee1d3be92734ad8"><div>Note that the <a href="#SkRect_width">width</a> cannot be represented as a 32-bit finite value.</div>

#### Example Output

~~~~
large is largest: true
large width inf
large is empty: false
large is sorted: true
large is finite: true
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_MakeLargest">MakeLargest</a> <a href="#SkIRect_isLargest">SkIRect::isLargest</a>

---

<a name="SkRect_isFinite"></a>
## isFinite

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
bool isFinite() const
</pre>

Returns true if all values in the rectangle are finite: <a href="undocumented#SK_ScalarMin">SK ScalarMin</a> or larger,
and <a href="undocumented#SK_ScalarMax">SK ScalarMax</a> or smaller. 

### Return Value

true if no member is infinite or <a href="undocumented#NaN">NaN</a>

### Example

<div><fiddle-embed name="7365c6553b830e6535cea0ce0d07be19">

#### Example Output

~~~~
largest is finite: true
large width inf
widest is finite: false
~~~~

</fiddle-embed></div>

### See Also

<a href="undocumented#SkScalarIsFinite">SkScalarIsFinite</a> <a href="undocumented#SkScalarIsNaN">SkScalarIsNaN</a>

---

<a name="SkRect_x"></a>
## x

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
SkScalar    x() const
</pre>

Returns <a href="#SkRect_left">left</a> edge of <a href="#Rect">Rect</a>, if sorted. Call <a href="#SkRect_isSorted">isSorted</a> to see if <a href="#Rect">Rect</a> is valid.
Call <a href="#SkRect_sort">sort</a> to reverse <a href="#SkRect_fLeft">fLeft</a> and <a href="#SkRect_fRight">fRight</a> if needed.

### Return Value

<a href="#SkRect_fLeft">fLeft</a>

### Example

<div><fiddle-embed name="23c77a35ac54a439a2989f840aa5cb99">

#### Example Output

~~~~
unsorted.fLeft: 15 unsorted.x(): 15
sorted.fLeft: 10 sorted.x(): 10
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_fLeft">fLeft</a> <a href="#SkRect_left">left</a> <a href="#SkRect_y">y</a> <a href="#SkIRect_x">SkIRect::x()</a>

---

<a name="SkRect_y"></a>
## y

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
SkScalar    y() const
</pre>

Returns <a href="#SkRect_top">top</a> edge of <a href="#Rect">Rect</a>, if sorted. Call <a href="#SkRect_isEmpty">isEmpty</a> to see if <a href="#Rect">Rect</a> may be invalid,
and <a href="#SkRect_sort">sort</a> to reverse <a href="#SkRect_fTop">fTop</a> and <a href="#SkRect_fBottom">fBottom</a> if needed.

### Return Value

<a href="#SkRect_fTop">fTop</a>

### Example

<div><fiddle-embed name="c653d9017983d2a047b1fee6a481d82b">

#### Example Output

~~~~
unsorted.fTop: 25 unsorted.y(): 25
sorted.fTop: 5 sorted.y(): 5
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_fTop">fTop</a> <a href="#SkRect_top">top</a> <a href="#SkRect_x">x</a> <a href="#SkIRect_y">SkIRect::y()</a>

---

<a name="SkRect_left"></a>
## left

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
SkScalar    left() const
</pre>

Returns <a href="#SkRect_left">left</a> edge of <a href="#Rect">Rect</a>, if sorted. Call <a href="#SkRect_isSorted">isSorted</a> to see if <a href="#Rect">Rect</a> is valid.
Call <a href="#SkRect_sort">sort</a> to reverse <a href="#SkRect_fLeft">fLeft</a> and <a href="#SkRect_fRight">fRight</a> if needed.

### Return Value

<a href="#SkRect_fLeft">fLeft</a>

### Example

<div><fiddle-embed name="900dc96c3549795a87036d6458c4fde6">

#### Example Output

~~~~
unsorted.fLeft: 15 unsorted.left(): 15
sorted.fLeft: 10 sorted.left(): 10
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_fLeft">fLeft</a> <a href="#SkRect_x">x</a> <a href="#SkIRect_left">SkIRect::left()</a>

---

<a name="SkRect_top"></a>
## top

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
SkScalar    top() const
</pre>

Returns <a href="#SkRect_top">top</a> edge of <a href="#Rect">Rect</a>, if sorted. Call <a href="#SkRect_isEmpty">isEmpty</a> to see if <a href="#Rect">Rect</a> may be invalid,
and <a href="#SkRect_sort">sort</a> to reverse <a href="#SkRect_fTop">fTop</a> and <a href="#SkRect_fBottom">fBottom</a> if needed.

### Return Value

<a href="#SkRect_fTop">fTop</a>

### Example

<div><fiddle-embed name="3cfc24b011aef1ca8ccb57c05711620c">

#### Example Output

~~~~
unsorted.fTop: 25 unsorted.top(): 25
sorted.fTop: 5 sorted.top(): 5
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_fTop">fTop</a> <a href="#SkRect_y">y</a> <a href="#SkIRect_top">SkIRect::top()</a>

---

<a name="SkRect_right"></a>
## right

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
SkScalar    right() const
</pre>

Returns <a href="#SkRect_right">right</a> edge of <a href="#Rect">Rect</a>, if sorted. Call <a href="#SkRect_isSorted">isSorted</a> to see if <a href="#Rect">Rect</a> is valid.
Call <a href="#SkRect_sort">sort</a> to reverse <a href="#SkRect_fLeft">fLeft</a> and <a href="#SkRect_fRight">fRight</a> if needed.

### Return Value

<a href="#SkRect_fRight">fRight</a>

### Example

<div><fiddle-embed name="ca3de7e5e292b3ad3633b1c39a31d3ab">

#### Example Output

~~~~
unsorted.fRight: 10 unsorted.right(): 10
sorted.fRight: 15 sorted.right(): 15
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_fRight">fRight</a> <a href="#SkIRect_right">SkIRect::right()</a>

---

<a name="SkRect_bottom"></a>
## bottom

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
SkScalar    bottom() const
</pre>

Returns <a href="#SkRect_bottom">bottom</a> edge of <a href="#Rect">Rect</a>, if sorted. Call <a href="#SkRect_isEmpty">isEmpty</a> to see if <a href="#Rect">Rect</a> may be invalid,
and <a href="#SkRect_sort">sort</a> to reverse <a href="#SkRect_fTop">fTop</a> and <a href="#SkRect_fBottom">fBottom</a> if needed.

### Return Value

<a href="#SkRect_fBottom">fBottom</a>

### Example

<div><fiddle-embed name="a98993a66616ae406d8bdc54adfb1411">

#### Example Output

~~~~
unsorted.fBottom: 5 unsorted.bottom(): 5
sorted.fBottom: 25 sorted.bottom(): 25
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_fBottom">fBottom</a> <a href="#SkIRect_bottom">SkIRect::bottom()</a>

---

<a name="SkRect_width"></a>
## width

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
SkScalar    width() const
</pre>

Returns span on the <a href="#SkRect_x">x</a>-axis. This does not check if <a href="#Rect">Rect</a> is sorted, or if
result fits in 32-bit float; result may be negative or infinity.

### Return Value

<a href="#SkRect_fRight">fRight</a> minus <a href="#SkRect_fLeft">fLeft</a>

### Example

<div><fiddle-embed name="11f8f0efe6291019fee0ac17844f6c1a"><div>Compare with <a href="#SkIRect_width">SkIRect::width()</a> example.</div>

#### Example Output

~~~~
unsorted width: -5
large width: 4294967296
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_height">height</a> <a href="#SkIRect_width">SkIRect::width()</a>

---

<a name="SkRect_height"></a>
## height

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
SkScalar    height() const
</pre>

Returns span on the <a href="#SkRect_y">y</a>-axis. This does not check if <a href="SkIRect_Reference#IRect">IRect</a> is sorted, or if
result fits in 32-bit float; result may be negative or infinity.

### Return Value

<a href="#SkRect_fBottom">fBottom</a> minus <a href="#SkRect_fTop">fTop</a>

### Example

<div><fiddle-embed name="39429e45f05240218ecd511443ab3e44"><div>Compare with <a href="#SkIRect_height">SkIRect::height()</a> example.</div>

#### Example Output

~~~~
unsorted height: -5
large height: 4294967296
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_width">width</a> <a href="#SkIRect_height">SkIRect::height()</a>

---

<a name="SkRect_centerX"></a>
## centerX

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
SkScalar    centerX() const
</pre>

Returns average of <a href="#SkRect_left">left</a> edge and <a href="#SkRect_right">right</a> edge. Result does not change if <a href="#Rect">Rect</a>
is sorted. Result may overflow to infinity if <a href="#Rect">Rect</a> is far from the origin.

### Return Value

midpoint in <a href="#SkRect_x">x</a>

### Example

<div><fiddle-embed name="d8439ba8d23a424fa032fb97147fd2d2">

#### Example Output

~~~~
left:  20 right:  41 centerX: 30.5
left:  20 right:  41 centerX: 30.5
left: -20 right: -41 centerX: -30.5
left: -41 right: -20 centerX: -30.5
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_centerY">centerY</a> <a href="#SkIRect_centerX">SkIRect::centerX</a>

---

<a name="SkRect_centerY"></a>
## centerY

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
SkScalar    centerY() const
</pre>

Returns average of <a href="#SkRect_top">top</a> edge and <a href="#SkRect_bottom">bottom</a> edge. Result does not change if <a href="#Rect">Rect</a>
is sorted. Result may overflow to infinity if <a href="#Rect">Rect</a> is far from the origin.

### Return Value

midpoint in <a href="#SkRect_y">y</a>

### Example

<div><fiddle-embed name="ebeeafafeb8fe39d5ffc9115b02c2340">

#### Example Output

~~~~
left: 2e+38 right: 3e+38 centerX: inf safe mid x: 2.5e+38
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_centerX">centerX</a> <a href="#SkIRect_centerY">SkIRect::centerY</a>

---

<a name="SkRect_equal_operator"></a>
## operator==

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
bool operator==(const SkRect& a, const SkRect& b)
</pre>

Returns true if all members in <a href="#SkRect_equal_operator_a">a</a>: <a href="#SkRect_fLeft">fLeft</a>, <a href="#SkRect_fTop">fTop</a>, <a href="#SkRect_fRight">fRight</a>, and <a href="#SkRect_fBottom">fBottom</a>; are  
equal to the corresponding members in <a href="#SkRect_equal_operator_b">b</a>.

<a href="#SkRect_equal_operator_a">a</a> and <a href="#SkRect_equal_operator_b">b</a> are not equal if either contain <a href="undocumented#NaN">NaN</a>. <a href="#SkRect_equal_operator_a">a</a> and <a href="#SkRect_equal_operator_b">b</a> are equal if members
contain zeroes <a href="#SkRect_width">width</a> different signs.

### Parameters

<table>  <tr>    <td><a name="SkRect_equal_operator_a"> <code><strong>a </strong></code> </a></td> <td>
<a href="#Rect">Rect</a> to compare</td>
  </tr>  <tr>    <td><a name="SkRect_equal_operator_b"> <code><strong>b </strong></code> </a></td> <td>
<a href="#Rect">Rect</a> to compare</td>
  </tr>
</table>

### Return Value

true if members are equal

### Example

<div><fiddle-embed name="c6c5b40cad7c3a839fdf576b380391a6">

#### Example Output

~~~~
tests are equal
{0, 0, 2, 2} == {-0, -0, 2, 2} and are numerically equal
{0, 0, 2, 2} == {-0, -0, 2, 2} and are numerically equal
{0, 0, 2, 2} == {-0, -0, 2, 2} and are numerically equal
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_notequal_operator">operator!=(const SkRect& a, const SkRect& b)</a>

---

<a name="SkRect_notequal_operator"></a>
## operator!=

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
bool operator!=(const SkRect& a, const SkRect& b)
</pre>

Returns true if any in <a href="#SkRect_notequal_operator_a">a</a>: <a href="#SkRect_fLeft">fLeft</a>, <a href="#SkRect_fTop">fTop</a>, <a href="#SkRect_fRight">fRight</a>, and <a href="#SkRect_fBottom">fBottom</a>; does not   
equal the corresponding members in <a href="#SkRect_notequal_operator_b">b</a>.

<a href="#SkRect_notequal_operator_a">a</a> and <a href="#SkRect_notequal_operator_b">b</a> are not equal if either contain <a href="undocumented#NaN">NaN</a>. <a href="#SkRect_notequal_operator_a">a</a> and <a href="#SkRect_notequal_operator_b">b</a> are equal if members
contain zeroes <a href="#SkRect_width">width</a> different signs.

### Parameters

<table>  <tr>    <td><a name="SkRect_notequal_operator_a"> <code><strong>a </strong></code> </a></td> <td>
<a href="#Rect">Rect</a> to compare</td>
  </tr>  <tr>    <td><a name="SkRect_notequal_operator_b"> <code><strong>b </strong></code> </a></td> <td>
<a href="#Rect">Rect</a> to compare</td>
  </tr>
</table>

### Return Value

true if members are not equal

### Example

<div><fiddle-embed name="286072f8c27ff15be9eb945fa38dc9f7">

#### Example Output

~~~~
test with NaN is not equal to itself
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_equal_operator">operator==(const SkRect& a, const SkRect& b)</a>

---

<a name="SkRect_toQuad"></a>
## toQuad

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void toQuad(SkPoint quad[4]) const
</pre>

Returns four points in <a href="#SkRect_toQuad_quad">quad</a> that enclose <a href="#Rect">Rect</a> ordered as: <a href="#SkRect_top">top</a>-<a href="#SkRect_left">left</a>, <a href="#SkRect_top">top</a>-<a href="#SkRect_right">right</a>,
<a href="#SkRect_bottom">bottom</a>-<a href="#SkRect_right">right</a>, <a href="#SkRect_bottom">bottom</a>-<a href="#SkRect_left">left</a>. 

### Parameters

<table>  <tr>    <td><a name="SkRect_toQuad_quad"> <code><strong>quad </strong></code> </a></td> <td>
storage for corners of <a href="#Rect">Rect</a></td>
  </tr>
</table>

### Example

<div><fiddle-embed name="59a6e7d202ac17ab80ec21b233e51f59">

#### Example Output

~~~~
rect: {1, 2, 3, 4}
corners: {1, 2} {3, 2} {3, 4} {1, 4}
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkPath_addRect">SkPath::addRect</a>

---

<a name="SkRect_setEmpty"></a>
## setEmpty

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void setEmpty()
</pre>

Sets <a href="#Rect">Rect</a> to (0, 0, 0, 0).

Many other rectangles are empty; if <a href="#SkRect_left">left</a> is equal to or greater than <a href="#SkRect_right">right</a>,
or if <a href="#SkRect_top">top</a> is equal to or greater than <a href="#SkRect_bottom">bottom</a>. Setting all members to zero
is a convenience, but does not designate a special empty rectangle.

### Example

<div><fiddle-embed name="2cf67542d45ef5d7a7efb673b651ff54">

#### Example Output

~~~~
rect: {3, 4, 1, 2} is empty
rect: {0, 0, 0, 0} is empty
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_MakeEmpty">MakeEmpty</a> <a href="#SkIRect_setEmpty">SkIRect::setEmpty</a>

---

<a name="SkRect_set"></a>
## set

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void set(const SkIRect& src)
</pre>

Sets <a href="#Rect">Rect</a> to <a href="#SkRect_set_src">src</a>, promoting <a href="#SkRect_set_src">src</a> members from integer to <a href="#Scalar">Scalar</a>.
Very large values in <a href="#SkRect_set_src">src</a> may lose precision. 

### Parameters

<table>  <tr>    <td><a name="SkRect_set_src"> <code><strong>src </strong></code> </a></td> <td>
integer <a href="#Rect">Rect</a></td>
  </tr>
</table>

### Example

<div><fiddle-embed name="a10ad8d97062bc3f40942f47e5108917">

#### Example Output

~~~~
i_rect: {3, 4, 1, 2}
f_rect: {3, 4, 1, 2}
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_setLTRB">setLTRB</a> <a href="undocumented#SkIntToScalar">SkIntToScalar</a>

---

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void set(SkScalar left, SkScalar top, SkScalar right, SkScalar bottom)
</pre>

Sets <a href="#Rect">Rect</a> to (<a href="#SkRect_left">left</a>, <a href="#SkRect_top">top</a>, <a href="#SkRect_right">right</a>, <a href="#SkRect_bottom">bottom</a>).
<a href="#SkRect_left">left</a> and <a href="#SkRect_right">right</a> are not sorted; <a href="#SkRect_left">left</a> is not necessarily less than <a href="#SkRect_right">right</a>.
<a href="#SkRect_top">top</a> and <a href="#SkRect_bottom">bottom</a> are not sorted; <a href="#SkRect_top">top</a> is not necessarily less than <a href="#SkRect_bottom">bottom</a>.

### Parameters

<table>  <tr>    <td><a name="SkRect_set_2_left"> <code><strong>left </strong></code> </a></td> <td>
stored in <a href="#SkRect_fLeft">fLeft</a></td>
  </tr>  <tr>    <td><a name="SkRect_set_2_top"> <code><strong>top </strong></code> </a></td> <td>
stored in <a href="#SkRect_fTop">fTop</a></td>
  </tr>  <tr>    <td><a name="SkRect_set_2_right"> <code><strong>right </strong></code> </a></td> <td>
stored in <a href="#SkRect_fRight">fRight</a></td>
  </tr>  <tr>    <td><a name="SkRect_set_2_bottom"> <code><strong>bottom </strong></code> </a></td> <td>
stored in <a href="#SkRect_fBottom">fBottom</a></td>
  </tr>
</table>

### Example

<div><fiddle-embed name="9b29ea460d69b4d47323fd9e3e17721e">

#### Example Output

~~~~
rect1: {3, 4, 1, 2}
rect2: {3, 4, 1, 2}
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_setLTRB">setLTRB</a> <a href="#SkRect_setXYWH">setXYWH</a> <a href="#SkIRect_set">SkIRect::set</a>

---

<a name="SkRect_setLTRB"></a>
## setLTRB

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void setLTRB(SkScalar left, SkScalar top, SkScalar right, SkScalar bottom)
</pre>

Sets <a href="#Rect">Rect</a> to (<a href="#SkRect_left">left</a>, <a href="#SkRect_top">top</a>, <a href="#SkRect_right">right</a>, <a href="#SkRect_bottom">bottom</a>).
<a href="#SkRect_left">left</a> and <a href="#SkRect_right">right</a> are not sorted; <a href="#SkRect_left">left</a> is not necessarily less than <a href="#SkRect_right">right</a>.
<a href="#SkRect_top">top</a> and <a href="#SkRect_bottom">bottom</a> are not sorted; <a href="#SkRect_top">top</a> is not necessarily less than <a href="#SkRect_bottom">bottom</a>.

### Parameters

<table>  <tr>    <td><a name="SkRect_setLTRB_left"> <code><strong>left </strong></code> </a></td> <td>
stored in <a href="#SkRect_fLeft">fLeft</a></td>
  </tr>  <tr>    <td><a name="SkRect_setLTRB_top"> <code><strong>top </strong></code> </a></td> <td>
stored in <a href="#SkRect_fTop">fTop</a></td>
  </tr>  <tr>    <td><a name="SkRect_setLTRB_right"> <code><strong>right </strong></code> </a></td> <td>
stored in <a href="#SkRect_fRight">fRight</a></td>
  </tr>  <tr>    <td><a name="SkRect_setLTRB_bottom"> <code><strong>bottom </strong></code> </a></td> <td>
stored in <a href="#SkRect_fBottom">fBottom</a></td>
  </tr>
</table>

### Example

<div><fiddle-embed name="70692838793454c8e045d6eaf7edcbff">

#### Example Output

~~~~
rect1: {3, 4, 1, 2}
rect2: {3, 4, 1, 2}
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_set">set</a> <a href="#SkRect_setXYWH">setXYWH</a> <a href="#SkIRect_set">SkIRect::set</a>

---

<a name="SkRect_iset"></a>
## iset

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void iset(int left, int top, int right, int bottom)
</pre>

Sets <a href="#Rect">Rect</a> to (<a href="#SkRect_left">left</a>, <a href="#SkRect_top">top</a>, <a href="#SkRect_right">right</a>, <a href="#SkRect_bottom">bottom</a>).
All parameters are promoted from integer to <a href="#Scalar">Scalar</a>.
<a href="#SkRect_left">left</a> and <a href="#SkRect_right">right</a> are not sorted; <a href="#SkRect_left">left</a> is not necessarily less than <a href="#SkRect_right">right</a>.
<a href="#SkRect_top">top</a> and <a href="#SkRect_bottom">bottom</a> are not sorted; <a href="#SkRect_top">top</a> is not necessarily less than <a href="#SkRect_bottom">bottom</a>.

### Parameters

<table>  <tr>    <td><a name="SkRect_iset_left"> <code><strong>left </strong></code> </a></td> <td>
promoted to <a href="undocumented#SkScalar">SkScalar</a> and stored in <a href="#SkRect_fLeft">fLeft</a></td>
  </tr>  <tr>    <td><a name="SkRect_iset_top"> <code><strong>top </strong></code> </a></td> <td>
promoted to <a href="undocumented#SkScalar">SkScalar</a> and stored in <a href="#SkRect_fTop">fTop</a></td>
  </tr>  <tr>    <td><a name="SkRect_iset_right"> <code><strong>right </strong></code> </a></td> <td>
promoted to <a href="undocumented#SkScalar">SkScalar</a> and stored in <a href="#SkRect_fRight">fRight</a></td>
  </tr>  <tr>    <td><a name="SkRect_iset_bottom"> <code><strong>bottom </strong></code> </a></td> <td>
promoted to <a href="undocumented#SkScalar">SkScalar</a> and stored in <a href="#SkRect_fBottom">fBottom</a></td>
  </tr>
</table>

### Example

<div><fiddle-embed name="18532f1aa90b76364fb8d7ea072f1892">

#### Example Output

~~~~
rect1: {3, 4, 1, 2}
rect2: {3, 4, 1, 2}
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_set">set</a> <a href="#SkRect_setLTRB">setLTRB</a> <a href="#SkIRect_set">SkIRect::set</a> <a href="undocumented#SkIntToScalar">SkIntToScalar</a>

---

<a name="SkRect_isetWH"></a>
## isetWH

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void isetWH(int width, int height)
</pre>

Sets <a href="#Rect">Rect</a> to (0, 0, <a href="#SkRect_width">width</a>, <a href="#SkRect_height">height</a>).
<a href="#SkRect_width">width</a> and <a href="#SkRect_height">height</a> may be zero or negative. <a href="#SkRect_width">width</a> and <a href="#SkRect_height">height</a> are promoted from
integer to <a href="undocumented#SkScalar">SkScalar</a>, large values may lose precision.

### Parameters

<table>  <tr>    <td><a name="SkRect_isetWH_width"> <code><strong>width </strong></code> </a></td> <td>
promoted to <a href="undocumented#SkScalar">SkScalar</a> and stored in <a href="#SkRect_fRight">fRight</a></td>
  </tr>  <tr>    <td><a name="SkRect_isetWH_height"> <code><strong>height </strong></code> </a></td> <td>
promoted to <a href="undocumented#SkScalar">SkScalar</a> and stored in <a href="#SkRect_fBottom">fBottom</a></td>
  </tr>
</table>

### Example

<div><fiddle-embed name="ee6000080fc7123214ea404018cf9176">

#### Example Output

~~~~
rect1: {0, 0, 1, 2}
rect2: {0, 0, 1, 2}
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_MakeWH">MakeWH</a> <a href="#SkRect_MakeXYWH">MakeXYWH</a> <a href="#SkRect_iset">iset</a> <a href="SkIRect_Reference#SkIRect">SkIRect</a>:<a href="#SkRect_MakeWH">MakeWH</a>

---

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void set(const SkPoint pts[], int count)
</pre>

Sets to bounds of <a href="SkPoint_Reference#Point">Point</a> array with <a href="#SkRect_set_3_count">count</a> entries. If <a href="#SkRect_set_3_count">count</a> is zero or smaller,
or if <a href="SkPoint_Reference#Point">Point</a> array <a href="#SkRect_contains">contains</a> an infinity or <a href="undocumented#NaN">NaN</a>, sets <a href="#Rect">Rect</a> to (0, 0, 0, 0).

Result is either empty or sorted: <a href="#SkRect_fLeft">fLeft</a> is less than or equal to <a href="#SkRect_fRight">fRight</a>, and
<a href="#SkRect_fTop">fTop</a> is less than or equal to <a href="#SkRect_fBottom">fBottom</a>.

### Parameters

<table>  <tr>    <td><a name="SkRect_set_3_pts"> <code><strong>pts </strong></code> </a></td> <td>
<a href="SkPoint_Reference#Point">Point</a> array</td>
  </tr>  <tr>    <td><a name="SkRect_set_3_count"> <code><strong>count </strong></code> </a></td> <td>
entries in array</td>
  </tr>
</table>

### Example

<div><fiddle-embed name="94295fa5197e21256171b99b4023dd48">

#### Example Output

~~~~
count: 0 rect: 0, 0, 0, 0
added:   3, 4 count: 1 rect: 3, 4, 3, 4
added:   1, 2 count: 2 rect: 1, 2, 3, 4
added:   5, 6 count: 3 rect: 1, 2, 5, 6
added: nan, 8 count: 4 rect: 0, 0, 0, 0
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_setBounds">setBounds</a> <a href="#SkRect_setBoundsCheck">setBoundsCheck</a> <a href="#SkPath_addPoly">SkPath::addPoly</a>

---

<a name="SkRect_setBounds"></a>
## setBounds

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void setBounds(const SkPoint pts[], int count)
</pre>

Sets to bounds of <a href="SkPoint_Reference#Point">Point</a> array with <a href="#SkRect_setBounds_count">count</a> entries. If <a href="#SkRect_setBounds_count">count</a> is zero or smaller,
or if <a href="SkPoint_Reference#Point">Point</a> array <a href="#SkRect_contains">contains</a> an infinity or <a href="undocumented#NaN">NaN</a>, sets to (0, 0, 0, 0).

Result is either empty or sorted: <a href="#SkRect_fLeft">fLeft</a> is less than or equal to <a href="#SkRect_fRight">fRight</a>, and
<a href="#SkRect_fTop">fTop</a> is less than or equal to <a href="#SkRect_fBottom">fBottom</a>.

### Parameters

<table>  <tr>    <td><a name="SkRect_setBounds_pts"> <code><strong>pts </strong></code> </a></td> <td>
<a href="SkPoint_Reference#Point">Point</a> array</td>
  </tr>  <tr>    <td><a name="SkRect_setBounds_count"> <code><strong>count </strong></code> </a></td> <td>
entries in array</td>
  </tr>
</table>

### Example

<div><fiddle-embed name="cf0da15f48aa54fd1889e7f913601710">

#### Example Output

~~~~
count: 0 rect: 0, 0, 0, 0
added:   3, 4 count: 1 rect: 3, 4, 3, 4
added:   1, 2 count: 2 rect: 1, 2, 3, 4
added:   5, 6 count: 3 rect: 1, 2, 5, 6
added: nan, 8 count: 4 rect: 0, 0, 0, 0
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_set">set</a> <a href="#SkRect_setBoundsCheck">setBoundsCheck</a> <a href="#SkPath_addPoly">SkPath::addPoly</a>

---

<a name="SkRect_setBoundsCheck"></a>
## setBoundsCheck

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
bool setBoundsCheck(const SkPoint pts[], int count)
</pre>

Sets to bounds of <a href="SkPoint_Reference#Point">Point</a> array with <a href="#SkRect_setBoundsCheck_count">count</a> entries. Returns false if <a href="#SkRect_setBoundsCheck_count">count</a> is
zero or smaller, or if <a href="SkPoint_Reference#Point">Point</a> array <a href="#SkRect_contains">contains</a> an infinity or <a href="undocumented#NaN">NaN</a>; in these cases
sets <a href="#Rect">Rect</a> to (0, 0, 0, 0).

Result is either empty or sorted: <a href="#SkRect_fLeft">fLeft</a> is less than or equal to <a href="#SkRect_fRight">fRight</a>, and
<a href="#SkRect_fTop">fTop</a> is less than or equal to <a href="#SkRect_fBottom">fBottom</a>.

### Parameters

<table>  <tr>    <td><a name="SkRect_setBoundsCheck_pts"> <code><strong>pts </strong></code> </a></td> <td>
<a href="SkPoint_Reference#Point">Point</a> array</td>
  </tr>  <tr>    <td><a name="SkRect_setBoundsCheck_count"> <code><strong>count </strong></code> </a></td> <td>
entries in array</td>
  </tr>
</table>

### Return Value

true if all <a href="SkPoint_Reference#Point">Point</a> values are finite

### Example

<div><fiddle-embed name="83d879b92683b15f9daaf0c9e71c5b35">

#### Example Output

~~~~
count: 0 rect: 0, 0, 0, 0 success: true
added:   3, 4 count: 1 rect: 3, 4, 3, 4 success: true
added:   1, 2 count: 2 rect: 1, 2, 3, 4 success: true
added:   5, 6 count: 3 rect: 1, 2, 5, 6 success: true
added: nan, 8 count: 4 rect: 0, 0, 0, 0 success: false
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_set">set</a> <a href="#SkRect_setBounds">setBounds</a> <a href="#SkPath_addPoly">SkPath::addPoly</a>

---

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void set(const SkPoint& p0, const SkPoint& p1)
</pre>

Sets bounds to the smallest <a href="#Rect">Rect</a> enclosing <a href="#Point">Points</a> <a href="#SkRect_set_4_p0">p0</a> and <a href="#SkRect_set_4_p1">p1</a>. The result is
sorted and may be empty. Does not check to see if values are finite.

### Parameters

<table>  <tr>    <td><a name="SkRect_set_4_p0"> <code><strong>p0 </strong></code> </a></td> <td>
corner to include</td>
  </tr>  <tr>    <td><a name="SkRect_set_4_p1"> <code><strong>p1 </strong></code> </a></td> <td>
corner to include</td>
  </tr>
</table>

### Example

<div><fiddle-embed name="ee72450381f768f3869153cdbeccdc3e"><div><a href="#SkRect_set_4_p0">p0</a> and <a href="#SkRect_set_4_p1">p1</a> may be swapped and have the same effect unless one <a href="#SkRect_contains">contains</a> <a href="undocumented#NaN">NaN</a>.</div></fiddle-embed></div>

### See Also

<a href="#SkRect_setBounds">setBounds</a> <a href="#SkRect_setBoundsCheck">setBoundsCheck</a>

---

<a name="SkRect_setXYWH"></a>
## setXYWH

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void setXYWH(SkScalar x, SkScalar y, SkScalar width, SkScalar height)
</pre>

Sets <a href="#Rect">Rect</a> to(<a href="#SkRect_x">x</a>, <a href="#SkRect_y">y</a>, <a href="#SkRect_x">x</a> + <a href="#SkRect_width">width</a>, <a href="#SkRect_y">y</a> + <a href="#SkRect_height">height</a>). 
Does not validate input;
<a href="#SkRect_width">width</a> or <a href="#SkRect_height">height</a> may be negative.

### Parameters

<table>  <tr>    <td><a name="SkRect_setXYWH_x"> <code><strong>x </strong></code> </a></td> <td>
stored in <a href="#SkRect_fLeft">fLeft</a></td>
  </tr>  <tr>    <td><a name="SkRect_setXYWH_y"> <code><strong>y </strong></code> </a></td> <td>
stored in <a href="#SkRect_fTop">fTop</a></td>
  </tr>  <tr>    <td><a name="SkRect_setXYWH_width"> <code><strong>width </strong></code> </a></td> <td>
added to <a href="#SkRect_x">x</a> and stored in <a href="#SkRect_fRight">fRight</a></td>
  </tr>  <tr>    <td><a name="SkRect_setXYWH_height"> <code><strong>height </strong></code> </a></td> <td>
added to <a href="#SkRect_y">y</a> and stored in <a href="#SkRect_fBottom">fBottom</a></td>
  </tr>
</table>

### Example

<div><fiddle-embed name="373cce4c61b9da0384b735b838765163">

#### Example Output

~~~~
rect: 5, 35, -10, 60  isEmpty: true
rect: -10, 35, 5, 60  isEmpty: false
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_MakeXYWH">MakeXYWH</a> <a href="#SkRect_setLTRB">setLTRB</a> <a href="#SkRect_set">set</a> <a href="#SkIRect_setXYWH">SkIRect::setXYWH</a>

---

<a name="SkRect_setWH"></a>
## setWH

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void setWH(SkScalar width, SkScalar height)
</pre>

Sets <a href="#Rect">Rect</a> to (0, 0, <a href="#SkRect_width">width</a>, <a href="#SkRect_height">height</a>). Does not validate input;
<a href="#SkRect_width">width</a> or <a href="#SkRect_height">height</a> may be negative.

### Parameters

<table>  <tr>    <td><a name="SkRect_setWH_width"> <code><strong>width </strong></code> </a></td> <td>
stored in <a href="#SkRect_fRight">fRight</a></td>
  </tr>  <tr>    <td><a name="SkRect_setWH_height"> <code><strong>height </strong></code> </a></td> <td>
stored in <a href="#SkRect_fBottom">fBottom</a></td>
  </tr>
</table>

### Example

<div><fiddle-embed name="9cb5fee17802fa49341f3707bdf5d235">

#### Example Output

~~~~
rect: 0, 0, -15, 25  isEmpty: true
rect: -15, 0, 0, 25  isEmpty: false
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_MakeWH">MakeWH</a> <a href="#SkRect_setXYWH">setXYWH</a> <a href="#SkRect_isetWH">isetWH</a>

---

<a name="SkRect_setLargest"></a>
## setLargest

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void setLargest()
</pre>

Sets rectangle <a href="#SkRect_left">left</a> and <a href="#SkRect_top">top</a> to most negative finite value, and sets
<a href="#SkRect_right">right</a> and <a href="#SkRect_bottom">bottom</a> to most positive finite value.

### Example

<div><fiddle-embed name="7a07b1914ade5c38cce6be417bc8aa4b">

#### Example Output

~~~~
MakeLargest isLargest: true
MakeLargest isFinite: true
outset a little isLargest: true
outset a little isFinite: true
outset a little more isLargest: false
outset a little more isFinite: false
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_MakeLargest">MakeLargest</a> <a href="#SkRect_isLargest">isLargest</a> <a href="#SkRect_setLargestInverted">setLargestInverted</a> <a href="undocumented#SK_ScalarMin">SK ScalarMin</a> <a href="undocumented#SK_ScalarMax">SK ScalarMax</a>

---

<a name="SkRect_setLargestInverted"></a>
## setLargestInverted

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void setLargestInverted()
</pre>

Sets rectangle <a href="#SkRect_left">left</a> and <a href="#SkRect_top">top</a> to most positive finite value, and sets
<a href="#SkRect_right">right</a> and <a href="#SkRect_bottom">bottom</a> to most negative finite value.

Use to initial <a href="#Rect">Rect</a> before one or more calls to <a href="#SkRect_growToInclude">growToInclude</a>.

### Example

<div><fiddle-embed name="394442eedd4f01e42c5a4905e832dd0c">

#### Example Output

~~~~
original rect: 3.40282e+38, 3.40282e+38, -3.40282e+38, -3.40282e+38  isEmpty: true
grown rect: 42, 24, 42, 24  isEmpty: true
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_growToInclude">growToInclude</a> <a href="#SkRect_setEmpty">setEmpty</a> <a href="#SkRect_setLargest">setLargest</a>

---

<a name="SkRect_makeOffset"></a>
## makeOffset

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
SkRect makeOffset(SkScalar dx, SkScalar dy) const
</pre>

Returns <a href="#Rect">Rect</a> <a href="#SkRect_offset">offset</a> by (<a href="#SkRect_makeOffset_dx">dx</a>, <a href="#SkRect_makeOffset_dy">dy</a>).

If <a href="#SkRect_makeOffset_dx">dx</a> is negative, <a href="#Rect">Rect</a> returned is moved to the <a href="#SkRect_left">left</a>.
If <a href="#SkRect_makeOffset_dx">dx</a> is positive, <a href="#Rect">Rect</a> returned is moved to the <a href="#SkRect_right">right</a>.
If <a href="#SkRect_makeOffset_dy">dy</a> is negative, <a href="#Rect">Rect</a> returned is moved upward.
If <a href="#SkRect_makeOffset_dy">dy</a> is positive, <a href="#Rect">Rect</a> returned is moved downward. 

### Parameters

<table>  <tr>    <td><a name="SkRect_makeOffset_dx"> <code><strong>dx </strong></code> </a></td> <td>
added to <a href="#SkRect_fLeft">fLeft</a> and <a href="#SkRect_fRight">fRight</a></td>
  </tr>  <tr>    <td><a name="SkRect_makeOffset_dy"> <code><strong>dy </strong></code> </a></td> <td>
added to <a href="#SkRect_fTop">fTop</a> and <a href="#SkRect_fBottom">fBottom</a></td>
  </tr>
</table>

### Return Value

<a href="#Rect">Rect</a> <a href="#SkRect_offset">offset</a> in <a href="#SkRect_x">x</a> or <a href="#SkRect_y">y</a>, with original <a href="#SkRect_width">width</a> and <a href="#SkRect_height">height</a>

### Example

<div><fiddle-embed name="98841ab0a932f99cccd8e6a34d94ba05">

#### Example Output

~~~~
rect: 10, 50, 20, 60  isEmpty: false
rect: 25, 82, 35, 92  isEmpty: false
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_offset">offset</a> <a href="#SkRect_makeInset">makeInset</a> <a href="#SkRect_makeOutset">makeOutset</a> <a href="#SkIRect_makeOffset">SkIRect::makeOffset</a>

---

<a name="SkRect_makeInset"></a>
## makeInset

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
SkRect makeInset(SkScalar dx, SkScalar dy) const
</pre>

Returns <a href="#Rect">Rect</a>, <a href="#SkRect_inset">inset</a> by (<a href="#SkRect_makeInset_dx">dx</a>, <a href="#SkRect_makeInset_dy">dy</a>).

If <a href="#SkRect_makeInset_dx">dx</a> is negative, <a href="#Rect">Rect</a> returned is wider.
If <a href="#SkRect_makeInset_dx">dx</a> is positive, <a href="#Rect">Rect</a> returned is narrower.
If <a href="#SkRect_makeInset_dy">dy</a> is negative, <a href="#Rect">Rect</a> returned is taller.
If <a href="#SkRect_makeInset_dy">dy</a> is positive, <a href="#Rect">Rect</a> returned is shorter. 

### Parameters

<table>  <tr>    <td><a name="SkRect_makeInset_dx"> <code><strong>dx </strong></code> </a></td> <td>
added to <a href="#SkRect_fLeft">fLeft</a> and subtracted from <a href="#SkRect_fRight">fRight</a></td>
  </tr>  <tr>    <td><a name="SkRect_makeInset_dy"> <code><strong>dy </strong></code> </a></td> <td>
added to <a href="#SkRect_fTop">fTop</a> and subtracted from <a href="#SkRect_fBottom">fBottom</a></td>
  </tr>
</table>

### Return Value

<a href="#Rect">Rect</a> <a href="#SkRect_inset">inset</a> symmetrically <a href="#SkRect_left">left</a> and <a href="#SkRect_right">right</a>, <a href="#SkRect_top">top</a> and <a href="#SkRect_bottom">bottom</a>

### Example

<div><fiddle-embed name="b8d32ab2f7ea3d4d5fb5a4ea2156f1c5">

#### Example Output

~~~~
rect: 10, 50, 20, 60  isEmpty: false
rect: 25, 82, 5, 28  isEmpty: true
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_inset">inset</a> <a href="#SkRect_makeOffset">makeOffset</a> <a href="#SkRect_makeOutset">makeOutset</a> <a href="#SkIRect_makeInset">SkIRect::makeInset</a>

---

<a name="SkRect_makeOutset"></a>
## makeOutset

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
SkRect makeOutset(SkScalar dx, SkScalar dy) const
</pre>

Returns <a href="#Rect">Rect</a>, <a href="#SkRect_outset">outset</a> by (<a href="#SkRect_makeOutset_dx">dx</a>, <a href="#SkRect_makeOutset_dy">dy</a>).

If <a href="#SkRect_makeOutset_dx">dx</a> is negative, <a href="#Rect">Rect</a> returned is narrower.
If <a href="#SkRect_makeOutset_dx">dx</a> is positive, <a href="#Rect">Rect</a> returned is wider.
If <a href="#SkRect_makeOutset_dy">dy</a> is negative, <a href="#Rect">Rect</a> returned is shorter.
If <a href="#SkRect_makeOutset_dy">dy</a> is positive, <a href="#Rect">Rect</a> returned is taller. 

### Parameters

<table>  <tr>    <td><a name="SkRect_makeOutset_dx"> <code><strong>dx </strong></code> </a></td> <td>
subtracted to <a href="#SkRect_fLeft">fLeft</a> and added from <a href="#SkRect_fRight">fRight</a></td>
  </tr>  <tr>    <td><a name="SkRect_makeOutset_dy"> <code><strong>dy </strong></code> </a></td> <td>
subtracted to <a href="#SkRect_fTop">fTop</a> and added from <a href="#SkRect_fBottom">fBottom</a></td>
  </tr>
</table>

### Return Value

<a href="#Rect">Rect</a> <a href="#SkRect_outset">outset</a> symmetrically <a href="#SkRect_left">left</a> and <a href="#SkRect_right">right</a>, <a href="#SkRect_top">top</a> and <a href="#SkRect_bottom">bottom</a>

### Example

<div><fiddle-embed name="87176fc60914cbca9c6a20998a033c24">

#### Example Output

~~~~
rect: 10, 50, 20, 60  isEmpty: false
rect: -5, 18, 35, 92  isEmpty: false
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_outset">outset</a> <a href="#SkRect_makeOffset">makeOffset</a> <a href="#SkRect_makeInset">makeInset</a> <a href="#SkIRect_makeOutset">SkIRect::makeOutset</a>

---

<a name="SkRect_offset"></a>
## offset

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void offset(SkScalar dx, SkScalar dy)
</pre>

Offsets <a href="#Rect">Rect</a> by adding <a href="#SkRect_offset_dx">dx</a> to <a href="#SkRect_fLeft">fLeft</a>, <a href="#SkRect_fRight">fRight</a>; and by adding <a href="#SkRect_offset_dy">dy</a> to <a href="#SkRect_fTop">fTop</a>, <a href="#SkRect_fBottom">fBottom</a>.

If <a href="#SkRect_offset_dx">dx</a> is negative, moves <a href="#Rect">Rect</a> to the <a href="#SkRect_left">left</a>.
If <a href="#SkRect_offset_dx">dx</a> is positive, moves <a href="#Rect">Rect</a> to the <a href="#SkRect_right">right</a>.
If <a href="#SkRect_offset_dy">dy</a> is negative, moves <a href="#Rect">Rect</a> upward.
If <a href="#SkRect_offset_dy">dy</a> is positive, moves <a href="#Rect">Rect</a> downward. 

### Parameters

<table>  <tr>    <td><a name="SkRect_offset_dx"> <code><strong>dx </strong></code> </a></td> <td>
<a href="#SkRect_offset">offset</a> added to <a href="#SkRect_fLeft">fLeft</a> and <a href="#SkRect_fRight">fRight</a></td>
  </tr>  <tr>    <td><a name="SkRect_offset_dy"> <code><strong>dy </strong></code> </a></td> <td>
<a href="#SkRect_offset">offset</a> added to <a href="#SkRect_fTop">fTop</a> and <a href="#SkRect_fBottom">fBottom</a></td>
  </tr>
</table>

### Example

<div><fiddle-embed name="04eb33f0fd376f2942ca5f1c7f6cbcfc">

#### Example Output

~~~~
rect: 15, 27, 55, 86
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_offsetTo">offsetTo</a> <a href="#SkRect_makeOffset">makeOffset</a> <a href="#SkIRect_offset">SkIRect::offset</a>

---

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void offset(const SkPoint& delta)
</pre>

Offsets <a href="#Rect">Rect</a> by adding <a href="#SkRect_offset_2_delta">delta</a>.fX to <a href="#SkRect_fLeft">fLeft</a>, <a href="#SkRect_fRight">fRight</a>; and by adding <a href="#SkRect_offset_2_delta">delta</a>.fY to
<a href="#SkRect_fTop">fTop</a>, <a href="#SkRect_fBottom">fBottom</a>.

If <a href="#SkRect_offset_2_delta">delta</a>.fX is negative, moves <a href="#Rect">Rect</a> to the <a href="#SkRect_left">left</a>.
If <a href="#SkRect_offset_2_delta">delta</a>.fX is positive, moves <a href="#Rect">Rect</a> to the <a href="#SkRect_right">right</a>.
If <a href="#SkRect_offset_2_delta">delta</a>.fY is negative, moves <a href="#Rect">Rect</a> upward.
If <a href="#SkRect_offset_2_delta">delta</a>.fY is positive, moves <a href="#Rect">Rect</a> downward. 

### Parameters

<table>  <tr>    <td><a name="SkRect_offset_2_delta"> <code><strong>delta </strong></code> </a></td> <td>
added to <a href="#Rect">Rect</a></td>
  </tr>
</table>

### Example

<div><fiddle-embed name="b24cf65561c98c1858a06c39f10fb797">

#### Example Output

~~~~
rect: 15, 27, 55, 86
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_offsetTo">offsetTo</a> <a href="#SkRect_makeOffset">makeOffset</a> <a href="#SkIRect_offset">SkIRect::offset</a>

---

<a name="SkRect_offsetTo"></a>
## offsetTo

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void offsetTo(SkScalar newX, SkScalar newY)
</pre>

Offsets <a href="#Rect">Rect</a> so that <a href="#SkRect_fLeft">fLeft</a> equals <a href="#SkRect_offsetTo_newX">newX</a>, and <a href="#SkRect_fTop">fTop</a> equals <a href="#SkRect_offsetTo_newY">newY</a>. <a href="#SkRect_width">width</a> and <a href="#SkRect_height">height</a>
are unchanged.

### Parameters

<table>  <tr>    <td><a name="SkRect_offsetTo_newX"> <code><strong>newX </strong></code> </a></td> <td>
stored in <a href="#SkRect_fLeft">fLeft</a>, preserving <a href="#SkRect_width">width</a></td>
  </tr>  <tr>    <td><a name="SkRect_offsetTo_newY"> <code><strong>newY </strong></code> </a></td> <td>
stored in <a href="#SkRect_fTop">fTop</a>, preserving <a href="#SkRect_height">height</a></td>
  </tr>
</table>

### Example

<div><fiddle-embed name="bedb04b7b3e1af3e8039f9cffe66989e">

#### Example Output

~~~~
rect: 15, 27, 55, 86
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_offset">offset</a> <a href="#SkRect_makeOffset">makeOffset</a> <a href="#SkRect_setXYWH">setXYWH</a> <a href="#SkIRect_offsetTo">SkIRect::offsetTo</a>

---

<a name="SkRect_inset"></a>
## inset

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void inset(SkScalar dx, SkScalar dy)
</pre>

Insets <a href="#Rect">Rect</a> by (<a href="#SkRect_inset_dx">dx</a>, <a href="#SkRect_inset_dy">dy</a>).

If <a href="#SkRect_inset_dx">dx</a> is positive, makes <a href="#Rect">Rect</a> narrower.
If <a href="#SkRect_inset_dx">dx</a> is negative, makes <a href="#Rect">Rect</a> wider.
If <a href="#SkRect_inset_dy">dy</a> is positive, makes <a href="#Rect">Rect</a> shorter.
If <a href="#SkRect_inset_dy">dy</a> is negative, makes <a href="#Rect">Rect</a> taller.

### Parameters

<table>  <tr>    <td><a name="SkRect_inset_dx"> <code><strong>dx </strong></code> </a></td> <td>
added to <a href="#SkRect_fLeft">fLeft</a> and subtracted from <a href="#SkRect_fRight">fRight</a></td>
  </tr>  <tr>    <td><a name="SkRect_inset_dy"> <code><strong>dy </strong></code> </a></td> <td>
added to <a href="#SkRect_fTop">fTop</a> and subtracted from <a href="#SkRect_fBottom">fBottom</a></td>
  </tr>
</table>

### Example

<div><fiddle-embed name="dae21340941dc6e4d048816dfd9f204c">

#### Example Output

~~~~
rect: 15, 27, 45, 60
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_outset">outset</a> <a href="#SkRect_makeInset">makeInset</a> <a href="#SkIRect_inset">SkIRect::inset</a>

---

<a name="SkRect_outset"></a>
## outset

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void outset(SkScalar dx, SkScalar dy)
</pre>

Outsets <a href="#Rect">Rect</a> by (<a href="#SkRect_outset_dx">dx</a>, <a href="#SkRect_outset_dy">dy</a>).

If <a href="#SkRect_outset_dx">dx</a> is positive, makes <a href="#Rect">Rect</a> wider.
If <a href="#SkRect_outset_dx">dx</a> is negative, makes <a href="#Rect">Rect</a> narrower.
If <a href="#SkRect_outset_dy">dy</a> is positive, makes <a href="#Rect">Rect</a> taller.
If <a href="#SkRect_outset_dy">dy</a> is negative, makes <a href="#Rect">Rect</a> shorter.

### Parameters

<table>  <tr>    <td><a name="SkRect_outset_dx"> <code><strong>dx </strong></code> </a></td> <td>
subtracted to <a href="#SkRect_fLeft">fLeft</a> and added from <a href="#SkRect_fRight">fRight</a></td>
  </tr>  <tr>    <td><a name="SkRect_outset_dy"> <code><strong>dy </strong></code> </a></td> <td>
subtracted to <a href="#SkRect_fTop">fTop</a> and added from <a href="#SkRect_fBottom">fBottom</a></td>
  </tr>
</table>

### Example

<div><fiddle-embed name="861f873ba660af8c8bf8b0b83d829cf4">

#### Example Output

~~~~
rect: 5, 1, 55, 86
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_inset">inset</a> <a href="#SkRect_makeOutset">makeOutset</a> <a href="#SkIRect_outset">SkIRect::outset</a>

---

# <a name="Intersection"></a> Intersection
<a href="#Rect">Rects</a> <a href="#SkRect_intersect">intersect</a> when they enclose a common area. To <a href="#SkRect_intersect">intersect</a>, each of the pair 
must describe area; <a href="#SkRect_fLeft">fLeft</a> is less than <a href="#SkRect_fRight">fRight</a>, and <a href="#SkRect_fTop">fTop</a> is less than <a href="#SkRect_fBottom">fBottom</a>;
empty() returns false. The intersection of <a href="#Rect">Rect</a> pair can be described by:

(max(a.fLeft, b.fLeft), max(a.fTop, b.fTop),
min(a.fRight, b.fRight), min(a.fBottom, b.fBottom)).

The intersection is only meaningful if the resulting <a href="#Rect">Rect</a> is not empty and
describes an area: <a href="#SkRect_fLeft">fLeft</a> is less than <a href="#SkRect_fRight">fRight</a>, and <a href="#SkRect_fTop">fTop</a> is less than <a href="#SkRect_fBottom">fBottom</a>.

<a name="SkRect_intersect"></a>
## intersect

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
bool intersect(const SkRect& r)
</pre>

Returns true if <a href="#Rect">Rect</a> <a href="#SkRect_intersects">intersects</a> <a href="#SkRect_intersect_r">r</a>, and sets <a href="#Rect">Rect</a> to intersection.
Returns false if <a href="#Rect">Rect</a> does not <a href="#SkRect_intersect">intersect</a> <a href="#SkRect_intersect_r">r</a>, and leaves <a href="#Rect">Rect</a> unchanged.

Returns false if either <a href="#SkRect_intersect_r">r</a> or <a href="#Rect">Rect</a> is empty, leaving <a href="#Rect">Rect</a> unchanged.

### Parameters

<table>  <tr>    <td><a name="SkRect_intersect_r"> <code><strong>r </strong></code> </a></td> <td>
limit of result</td>
  </tr>
</table>

### Return Value

true if <a href="#SkRect_intersect_r">r</a> and <a href="#Rect">Rect</a> have area in common

### Example

<div><fiddle-embed name="70e5b3979fc8a31eda070cfed91bc271"><div>Two <a href="undocumented#SkDebugf">SkDebugf</a> calls are required. If the calls are combined, their arguments
may not be evaluated in <a href="#SkRect_left">left</a> to <a href="#SkRect_right">right</a> order: the printed intersection may
be before or after the call to <a href="#SkRect_intersect">intersect</a>.</div>

#### Example Output

~~~~
intersection: 30, 60, 50, 80
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_intersects">intersects</a> <a href="#SkRect_Intersects">Intersects</a> <a href="#SkRect_join">join</a> <a href="#SkIRect_intersect">SkIRect::intersect</a>

---

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
bool intersect(SkScalar left, SkScalar top, SkScalar right, SkScalar bottom)
</pre>

Constructs <a href="#Rect">Rect</a> to <a href="#SkRect_intersect">intersect</a> from (<a href="#SkRect_left">left</a>, <a href="#SkRect_top">top</a>, <a href="#SkRect_right">right</a>, <a href="#SkRect_bottom">bottom</a>). Does not <a href="#SkRect_sort">sort</a>
construction.

Returns true if <a href="#Rect">Rect</a> <a href="#SkRect_intersects">intersects</a> construction, and sets <a href="#Rect">Rect</a> to intersection.
Returns false if <a href="#Rect">Rect</a> does not <a href="#SkRect_intersect">intersect</a> construction, and leaves <a href="#Rect">Rect</a> unchanged.

Returns false if either construction or <a href="#Rect">Rect</a> is empty, leaving <a href="#Rect">Rect</a> unchanged.

### Parameters

<table>  <tr>    <td><a name="SkRect_intersect_2_left"> <code><strong>left </strong></code> </a></td> <td>
<a href="#SkRect_x">x</a> minimum of constructed <a href="#Rect">Rect</a></td>
  </tr>  <tr>    <td><a name="SkRect_intersect_2_top"> <code><strong>top </strong></code> </a></td> <td>
<a href="#SkRect_y">y</a> minimum of constructed <a href="#Rect">Rect</a></td>
  </tr>  <tr>    <td><a name="SkRect_intersect_2_right"> <code><strong>right </strong></code> </a></td> <td>
<a href="#SkRect_x">x</a> maximum of constructed <a href="#Rect">Rect</a></td>
  </tr>  <tr>    <td><a name="SkRect_intersect_2_bottom"> <code><strong>bottom </strong></code> </a></td> <td>
<a href="#SkRect_y">y</a> maximum of constructed <a href="#Rect">Rect</a></td>
  </tr>
</table>

### Return Value

true if construction and <a href="#Rect">Rect</a> have area in common

### Example

<div><fiddle-embed name="9f06dad5e6c712f7a2c149d075e816d2"><div>Two <a href="undocumented#SkDebugf">SkDebugf</a> calls are required. If the calls are combined, their arguments
may not be evaluated in <a href="#SkRect_left">left</a> to <a href="#SkRect_right">right</a> order: the printed intersection may
be before or after the call to <a href="#SkRect_intersect">intersect</a>.</div>

#### Example Output

~~~~
intersection: 30, 60, 50, 80
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_intersects">intersects</a> <a href="#SkRect_Intersects">Intersects</a> <a href="#SkRect_join">join</a> <a href="#SkIRect_intersect">SkIRect::intersect</a>

---

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
bool SK_WARN_UNUSED_RESULT intersect(const SkRect& a, const SkRect& b)
</pre>

Returns true if <a href="#SkRect_intersect_3_a">a</a> <a href="#SkRect_intersects">intersects</a> <a href="#SkRect_intersect_3_b">b</a>, and sets <a href="#Rect">Rect</a> to intersection.
Returns false if <a href="#SkRect_intersect_3_a">a</a> does not <a href="#SkRect_intersect">intersect</a> <a href="#SkRect_intersect_3_b">b</a>, and leaves <a href="#Rect">Rect</a> unchanged.

Returns false if either <a href="#SkRect_intersect_3_a">a</a> or <a href="#SkRect_intersect_3_b">b</a> is empty, leaving <a href="#Rect">Rect</a> unchanged.

### Parameters

<table>  <tr>    <td><a name="SkRect_intersect_3_a"> <code><strong>a </strong></code> </a></td> <td>
<a href="#Rect">Rect</a> to <a href="#SkRect_intersect">intersect</a></td>
  </tr>  <tr>    <td><a name="SkRect_intersect_3_b"> <code><strong>b </strong></code> </a></td> <td>
<a href="#Rect">Rect</a> to <a href="#SkRect_intersect">intersect</a></td>
  </tr>
</table>

### Return Value

true if <a href="#SkRect_intersect_3_a">a</a> and <a href="#SkRect_intersect_3_b">b</a> have area in common

### Example

<div><fiddle-embed name="d610437a65dd3e952719efe605cbd0c7">

#### Example Output

~~~~
intersection: 30, 60, 50, 80
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_intersects">intersects</a> <a href="#SkRect_Intersects">Intersects</a> <a href="#SkRect_join">join</a> <a href="#SkIRect_intersect">SkIRect::intersect</a>

---

<a name="SkRect_intersects"></a>
## intersects

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
bool intersects(SkScalar left, SkScalar top, SkScalar right, SkScalar bottom) const
</pre>

Constructs <a href="#Rect">Rect</a> to <a href="#SkRect_intersect">intersect</a> from (<a href="#SkRect_left">left</a>, <a href="#SkRect_top">top</a>, <a href="#SkRect_right">right</a>, <a href="#SkRect_bottom">bottom</a>). Does not <a href="#SkRect_sort">sort</a>
construction.

Returns true if <a href="#Rect">Rect</a> <a href="#SkRect_intersects">intersects</a> construction.
Returns false if either construction or <a href="#Rect">Rect</a> is empty, or do not <a href="#SkRect_intersect">intersect</a>.

### Parameters

<table>  <tr>    <td><a name="SkRect_intersects_left"> <code><strong>left </strong></code> </a></td> <td>
<a href="#SkRect_x">x</a> minimum of constructed <a href="#Rect">Rect</a></td>
  </tr>  <tr>    <td><a name="SkRect_intersects_top"> <code><strong>top </strong></code> </a></td> <td>
<a href="#SkRect_y">y</a> minimum of constructed <a href="#Rect">Rect</a></td>
  </tr>  <tr>    <td><a name="SkRect_intersects_right"> <code><strong>right </strong></code> </a></td> <td>
<a href="#SkRect_x">x</a> maximum of constructed <a href="#Rect">Rect</a></td>
  </tr>  <tr>    <td><a name="SkRect_intersects_bottom"> <code><strong>bottom </strong></code> </a></td> <td>
<a href="#SkRect_y">y</a> maximum of constructed <a href="#Rect">Rect</a></td>
  </tr>
</table>

### Return Value

true if construction and <a href="#Rect">Rect</a> have area in common

### Example

<div><fiddle-embed name="7145dc17ebce4f54e892102f6c98e811">

#### Example Output

~~~~
intersection
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_intersect">intersect</a> <a href="#SkRect_Intersects">Intersects</a> <a href="#SkIRect_Intersects">SkIRect::Intersects</a>

---

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
bool intersects(const SkRect& r) const
</pre>

Returns true if <a href="#Rect">Rect</a> <a href="#SkRect_intersects">intersects</a> <a href="#SkRect_intersects_2_r">r</a>.
Returns false if either <a href="#SkRect_intersects_2_r">r</a> or <a href="#Rect">Rect</a> is empty, or do not <a href="#SkRect_intersect">intersect</a>.

### Parameters

<table>  <tr>    <td><a name="SkRect_intersects_2_r"> <code><strong>r </strong></code> </a></td> <td>
<a href="#Rect">Rect</a> to <a href="#SkRect_intersect">intersect</a></td>
  </tr>
</table>

### Return Value

true if <a href="#SkRect_intersects_2_r">r</a> and <a href="#Rect">Rect</a> have area in common

### Example

<div><fiddle-embed name="ca37b4231b21eb8296cb19ba9e0c781b">

#### Example Output

~~~~
intersection
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_intersect">intersect</a> <a href="#SkRect_Intersects">Intersects</a> <a href="#SkIRect_Intersects">SkIRect::Intersects</a>

---

<a name="SkRect_Intersects"></a>
## Intersects

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
static bool Intersects(const SkRect& a, const SkRect& b)
</pre>

Returns true if <a href="#SkRect_Intersects_a">a</a> <a href="#SkRect_intersects">intersects</a> <a href="#SkRect_Intersects_b">b</a>.
Returns false if either <a href="#SkRect_Intersects_a">a</a> or <a href="#SkRect_Intersects_b">b</a> is empty, or do not <a href="#SkRect_intersect">intersect</a>.

### Parameters

<table>  <tr>    <td><a name="SkRect_Intersects_a"> <code><strong>a </strong></code> </a></td> <td>
<a href="#Rect">Rect</a> to <a href="#SkRect_intersect">intersect</a></td>
  </tr>  <tr>    <td><a name="SkRect_Intersects_b"> <code><strong>b </strong></code> </a></td> <td>
<a href="#Rect">Rect</a> to <a href="#SkRect_intersect">intersect</a></td>
  </tr>
</table>

### Return Value

true if <a href="#SkRect_Intersects_a">a</a> and <a href="#SkRect_Intersects_b">b</a> have area in common

### Example

<div><fiddle-embed name="795061764b10c9e05efb466c9cb60644">

#### Example Output

~~~~
intersection
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_intersect">intersect</a> <a href="#SkRect_intersects">intersects</a> <a href="#SkIRect_Intersects">SkIRect::Intersects</a>

---

<a name="SkRect_join"></a>
## join

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void join(SkScalar left, SkScalar top, SkScalar right, SkScalar bottom)
</pre>

Constructs <a href="#Rect">Rect</a> to <a href="#SkRect_intersect">intersect</a> from (<a href="#SkRect_left">left</a>, <a href="#SkRect_top">top</a>, <a href="#SkRect_right">right</a>, <a href="#SkRect_bottom">bottom</a>). Does not <a href="#SkRect_sort">sort</a>
construction.

Sets <a href="#Rect">Rect</a> to the union of itself and the construction.

Has no effect if construction is empty. Otherwise, if <a href="#Rect">Rect</a> is empty, sets
<a href="#Rect">Rect</a> to construction.

### Parameters

<table>  <tr>    <td><a name="SkRect_join_left"> <code><strong>left </strong></code> </a></td> <td>
<a href="#SkRect_x">x</a> minimum of constructed <a href="#Rect">Rect</a></td>
  </tr>  <tr>    <td><a name="SkRect_join_top"> <code><strong>top </strong></code> </a></td> <td>
<a href="#SkRect_y">y</a> minimum of constructed <a href="#Rect">Rect</a></td>
  </tr>  <tr>    <td><a name="SkRect_join_right"> <code><strong>right </strong></code> </a></td> <td>
<a href="#SkRect_x">x</a> maximum of constructed <a href="#Rect">Rect</a></td>
  </tr>  <tr>    <td><a name="SkRect_join_bottom"> <code><strong>bottom </strong></code> </a></td> <td>
<a href="#SkRect_y">y</a> maximum of constructed <a href="#Rect">Rect</a></td>
  </tr>
</table>

### Example

<div><fiddle-embed name="afa9c6b4d05bb669db07fe0b7b97e6aa">

#### Example Output

~~~~
join: 10, 20, 55, 65
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_joinNonEmptyArg">joinNonEmptyArg</a> <a href="#SkRect_joinPossiblyEmptyRect">joinPossiblyEmptyRect</a> <a href="#SkIRect_join">SkIRect::join</a>

---

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void join(const SkRect& r)
</pre>

Sets <a href="#Rect">Rect</a> to the union of itself and <a href="#SkRect_join_2_r">r</a>.

Has no effect if <a href="#SkRect_join_2_r">r</a> is empty. Otherwise, if <a href="#Rect">Rect</a> is empty, sets
<a href="#Rect">Rect</a> to <a href="#SkRect_join_2_r">r</a>.

### Parameters

<table>  <tr>    <td><a name="SkRect_join_2_r"> <code><strong>r </strong></code> </a></td> <td>
expansion <a href="#Rect">Rect</a></td>
  </tr>
</table>

### Example

<div><fiddle-embed name="26500032494cf93c5fa3423110fe82af">

#### Example Output

~~~~
join: 10, 20, 55, 65
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_joinNonEmptyArg">joinNonEmptyArg</a> <a href="#SkRect_joinPossiblyEmptyRect">joinPossiblyEmptyRect</a> <a href="#SkIRect_join">SkIRect::join</a>

---

<a name="SkRect_joinNonEmptyArg"></a>
## joinNonEmptyArg

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void joinNonEmptyArg(const SkRect& r)
</pre>

Sets <a href="#Rect">Rect</a> to the union of itself and <a href="#SkRect_joinNonEmptyArg_r">r</a>.

Asserts if <a href="#SkRect_joinNonEmptyArg_r">r</a> is empty and <a href="undocumented#SK_DEBUG">SK DEBUG</a> is defined.
If <a href="#Rect">Rect</a> is empty, sets <a href="#Rect">Rect</a> to <a href="#SkRect_joinNonEmptyArg_r">r</a>.

May produce incorrect results if <a href="#SkRect_joinNonEmptyArg_r">r</a> is empty.

### Parameters

<table>  <tr>    <td><a name="SkRect_joinNonEmptyArg_r"> <code><strong>r </strong></code> </a></td> <td>
expansion <a href="#Rect">Rect</a></td>
  </tr>
</table>

### Example

<div><fiddle-embed name="88439de2aa0911262c60c0eb506396cb"><div>Since <a href="#Rect">Rect</a> is not sorted, first result is copy of toJoin.</div>

#### Example Output

~~~~
rect: 50, 60, 55, 65
sorted: 10, 0, 55, 100
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_join">join</a> <a href="#SkRect_joinPossiblyEmptyRect">joinPossiblyEmptyRect</a> <a href="#SkIRect_join">SkIRect::join</a>

---

<a name="SkRect_joinPossiblyEmptyRect"></a>
## joinPossiblyEmptyRect

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void joinPossiblyEmptyRect(const SkRect& r)
</pre>

Sets <a href="#Rect">Rect</a> to the union of itself and the construction.

May produce incorrect results if <a href="#Rect">Rect</a> or <a href="#SkRect_joinPossiblyEmptyRect_r">r</a> is empty.

### Parameters

<table>  <tr>    <td><a name="SkRect_joinPossiblyEmptyRect_r"> <code><strong>r </strong></code> </a></td> <td>
expansion <a href="#Rect">Rect</a></td>
  </tr>
</table>

### Example

<div><fiddle-embed name="a476548d0001296afd8e58c1eba1b70b"><div>Since <a href="#Rect">Rect</a> is not sorted, first result is not useful.</div>

#### Example Output

~~~~
rect: 10, 60, 55, 65
sorted: 10, 0, 55, 100
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_joinNonEmptyArg">joinNonEmptyArg</a> <a href="#SkRect_join">join</a> <a href="#SkIRect_join">SkIRect::join</a>

---

<a name="SkRect_growToInclude"></a>
## growToInclude

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void growToInclude(SkPoint pt)
</pre>

Grows <a href="#Rect">Rect</a> to include (<a href="#SkRect_growToInclude_pt">pt</a>.fX, <a href="#SkRect_growToInclude_pt">pt</a>.fY), modifying it so that:

<a href="#SkRect_fLeft">fLeft</a> <= <a href="#SkRect_growToInclude_pt">pt</a>.fX <= <a href="#SkRect_fRight">fRight</a> && <a href="#SkRect_fTop">fTop</a> <= <a href="#SkRect_growToInclude_pt">pt</a>.fY <= <a href="#SkRect_fBottom">fBottom</a>.

If <a href="#Rect">Rect</a> is initialized with <a href="#SkRect_setLargestInverted">setLargestInverted</a>, then <a href="#Rect">Rect</a> will contain bounds of
<a href="#Point">Points</a> after one or more calls. In this case, <a href="#Rect">Rect</a> is empty after first call.

### Parameters

<table>  <tr>    <td><a name="SkRect_growToInclude_pt"> <code><strong>pt </strong></code> </a></td> <td>
<a href="SkPoint_Reference#Point">Point</a> to include</td>
  </tr>
</table>

### Example

<div><fiddle-embed name="c2855b1bc92b4e332f6edd968065e4f0">

#### Example Output

~~~~
rect: 42, 24, 42, 24  isEmpty: true
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_setLargestInverted">setLargestInverted</a> <a href="#SkRect_join">join</a>

---

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void growToInclude(const SkPoint pts[], int count)
</pre>

For each of <a href="#SkRect_growToInclude_2_count">count</a> <a href="SkPoint_Reference#Point">Point</a> in <a href="#SkRect_growToInclude_2_pts">pts</a>, grows <a href="#Rect">Rect</a> to include (pt.fX, pt.fY), modifying
it so that:
<a href="#SkRect_fLeft">fLeft</a> <= pt.fX <= <a href="#SkRect_fRight">fRight</a> && <a href="#SkRect_fTop">fTop</a> <= pt.fY <= <a href="#SkRect_fBottom">fBottom</a>.

If <a href="#Rect">Rect</a> is initialized with <a href="#SkRect_setLargestInverted">setLargestInverted</a>, then <a href="#Rect">Rect</a> will contain bounds of
<a href="#Point">Points</a> after one or more calls. In this case, <a href="#Rect">Rect</a> is empty after first call.

### Parameters

<table>  <tr>    <td><a name="SkRect_growToInclude_2_pts"> <code><strong>pts </strong></code> </a></td> <td>
<a href="SkPoint_Reference#Point">Point</a> array</td>
  </tr>  <tr>    <td><a name="SkRect_growToInclude_2_count"> <code><strong>count </strong></code> </a></td> <td>
number of points in array</td>
  </tr>
</table>

### Example

<div><fiddle-embed name="73082d5aea2026cae4c42ec587417df2">

#### Example Output

~~~~
rect: 30, 50, 40, 60
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_setLargestInverted">setLargestInverted</a> <a href="#SkRect_join">join</a>

---

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void growToInclude(const SkPoint pts[], size_t stride, int count)
</pre>

For each of <a href="#SkRect_growToInclude_3_count">count</a> <a href="SkPoint_Reference#Point">Point</a> in <a href="#SkRect_growToInclude_3_pts">pts</a>, grows <a href="#Rect">Rect</a> to include (pt.fX, pt.fY), modifying
it so that:
<a href="#SkRect_fLeft">fLeft</a> <= pt.fX <= <a href="#SkRect_fRight">fRight</a> && <a href="#SkRect_fTop">fTop</a> <= pt.fY <= <a href="#SkRect_fBottom">fBottom</a>. 

<a href="SkPoint_Reference#Point">Point</a> may be followed with other data in each array element. <a href="#SkRect_growToInclude_3_stride">stride</a> is number
of bytes in element; the interval to skip to advance from one <a href="SkPoint_Reference#Point">Point</a> to
the next.

If <a href="#Rect">Rect</a> is initialized with <a href="#SkRect_setLargestInverted">setLargestInverted</a>, then <a href="#Rect">Rect</a> will contain bounds of
<a href="#Point">Points</a> after one or more calls. In this case, <a href="#Rect">Rect</a> is empty after first call.

### Parameters

<table>  <tr>    <td><a name="SkRect_growToInclude_3_pts"> <code><strong>pts </strong></code> </a></td> <td>
array of elements beginning with <a href="SkPoint_Reference#Point">Point</a></td>
  </tr>  <tr>    <td><a name="SkRect_growToInclude_3_stride"> <code><strong>stride </strong></code> </a></td> <td>
size of <a href="#SkRect_growToInclude_3_pts">pts</a> elements in 32-bit words; zero or greater</td>
  </tr>  <tr>    <td><a name="SkRect_growToInclude_3_count"> <code><strong>count </strong></code> </a></td> <td>
number of elements in array</td>
  </tr>
</table>

### Example

<div><fiddle-embed name="6dbf21017c60b76cae3379fd9101342a">

#### Example Output

~~~~
#Volatile
rect: 30, 50, 40, 60
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_setLargestInverted">setLargestInverted</a> <a href="#SkRect_join">join</a>

---

<a name="SkRect_contains"></a>
## contains

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
bool contains(const SkRect& r) const
</pre>

Returns true if <a href="#Rect">Rect</a> <a href="#SkRect_contains">contains</a> <a href="#SkRect_contains_r">r</a>.
Returns false if <a href="#Rect">Rect</a> is empty or <a href="#SkRect_contains_r">r</a> is empty.

<a href="#Rect">Rect</a> <a href="#SkRect_contains">contains</a> <a href="#SkRect_contains_r">r</a> when <a href="#Rect">Rect</a> area completely includes <a href="#SkRect_contains_r">r</a> area.

### Parameters

<table>  <tr>    <td><a name="SkRect_contains_r"> <code><strong>r </strong></code> </a></td> <td>
<a href="#Rect">Rect</a> contained</td>
  </tr>
</table>

### Return Value

true if all sides of <a href="#Rect">Rect</a> are outside <a href="#SkRect_contains_r">r</a>

### Example

<div><fiddle-embed name="92f9e6aa5bb76791139a24cf7d8df99e">

#### Example Output

~~~~
rect: (30, 50, 40, 60) contains (30, 50, 31, 51)
rect: (30, 50, 40, 60) does not contain (39, 49, 40, 50)
rect: (30, 50, 40, 60) does not contain (29, 59, 30, 60)
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkIRect_contains">SkIRect::contains</a>

---

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
bool contains(const SkIRect& r) const
</pre>

Returns true if <a href="#Rect">Rect</a> <a href="#SkRect_contains">contains</a> <a href="#SkRect_contains_2_r">r</a>.
Returns false if <a href="#Rect">Rect</a> is empty or <a href="#SkRect_contains_2_r">r</a> is empty.

<a href="#Rect">Rect</a> <a href="#SkRect_contains">contains</a> <a href="#SkRect_contains_2_r">r</a> when <a href="#Rect">Rect</a> area completely includes <a href="#SkRect_contains_2_r">r</a> area.

### Parameters

<table>  <tr>    <td><a name="SkRect_contains_2_r"> <code><strong>r </strong></code> </a></td> <td>
<a href="SkIRect_Reference#IRect">IRect</a> contained</td>
  </tr>
</table>

### Return Value

true if all sides of <a href="#Rect">Rect</a> are outside <a href="#SkRect_contains_2_r">r</a>

### Example

<div><fiddle-embed name="dd58b699551dd44026a2c6386be27d88">

#### Example Output

~~~~
rect: (30, 50, 40, 60) contains (30, 50, 31, 51)
rect: (30, 50, 40, 60) does not contain (39, 49, 40, 50)
rect: (30, 50, 40, 60) does not contain (29, 59, 30, 60)
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkIRect_contains">SkIRect::contains</a>

---

# <a name="Round"></a> Round

<a name="SkRect_round"></a>
## round

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void round(SkIRect* dst) const
</pre>

Sets <a href="SkIRect_Reference#IRect">IRect</a> by adding 0.5 and discarding the fractional portion of <a href="#Rect">Rect</a>
members, using(.

### Parameters

<table>  <tr>    <td><a name="SkRect_round_dst"> <code><strong>dst </strong></code> </a></td> <td>
storage for <a href="SkIRect_Reference#IRect">IRect</a></td>
  </tr>
</table>

### Example

<div><fiddle-embed name="8b9e5a9af0a9b878f76919534d88f41e">

#### Example Output

~~~~
round: 31, 51, 41, 61
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_roundIn">roundIn</a> <a href="#SkRect_roundOut">roundOut</a> <a href="undocumented#SkScalarRoundToInt">SkScalarRoundToInt</a>

---

<a name="SkRect_roundOut"></a>
## roundOut

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void roundOut(SkIRect* dst) const
</pre>

Sets <a href="SkIRect_Reference#IRect">IRect</a> by discarding the fractional portion of <a href="#SkRect_fLeft">fLeft</a> and <a href="#SkRect_fTop">fTop</a>; and
rounding up <a href="#SkRect_fRight">fRight</a> and FBottom, using(.

### Parameters

<table>  <tr>    <td><a name="SkRect_roundOut_dst"> <code><strong>dst </strong></code> </a></td> <td>
storage for <a href="SkIRect_Reference#IRect">IRect</a></td>
  </tr>
</table>

### Example

<div><fiddle-embed name="0bd13d7e6426ae7a3befa2ab151ac5fc">

#### Example Output

~~~~
round: 30, 50, 41, 61
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_roundIn">roundIn</a> <a href="#SkRect_round">round</a> <a href="undocumented#SkScalarRoundToInt">SkScalarRoundToInt</a>

---

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void roundOut(SkRect* dst) const
</pre>

Sets <a href="#Rect">Rect</a> by discarding the fractional portion of <a href="#SkRect_fLeft">fLeft</a> and <a href="#SkRect_fTop">fTop</a>; and
rounding up <a href="#SkRect_fRight">fRight</a> and FBottom, using(.

### Parameters

<table>  <tr>    <td><a name="SkRect_roundOut_2_dst"> <code><strong>dst </strong></code> </a></td> <td>
storage for <a href="#Rect">Rect</a></td>
  </tr>
</table>

### Example

<div><fiddle-embed name="e09a6a12869a8ac21e9c2af98a5bb686">

#### Example Output

~~~~
round: 30, 50, 41, 61
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_roundIn">roundIn</a> <a href="#SkRect_round">round</a> <a href="undocumented#SkScalarRoundToInt">SkScalarRoundToInt</a>

---

<a name="SkRect_roundIn"></a>
## roundIn

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void roundIn(SkIRect* dst) const
</pre>

Sets <a href="#Rect">Rect</a> by rounding up <a href="#SkRect_fLeft">fLeft</a> and <a href="#SkRect_fTop">fTop</a>; and
discarding the fractional portion of <a href="#SkRect_fRight">fRight</a> and FBottom, using(.

### Parameters

<table>  <tr>    <td><a name="SkRect_roundIn_dst"> <code><strong>dst </strong></code> </a></td> <td>
storage for <a href="SkIRect_Reference#IRect">IRect</a></td>
  </tr>
</table>

### Example

<div><fiddle-embed name="abb337da8fc1891f016c61258681c64c">

#### Example Output

~~~~
round: 31, 51, 40, 60
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_roundOut">roundOut</a> <a href="#SkRect_round">round</a> <a href="undocumented#SkScalarRoundToInt">SkScalarRoundToInt</a>

---

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
SkIRect round() const
</pre>

Returns <a href="SkIRect_Reference#IRect">IRect</a> by adding 0.5 and discarding the fractional portion of <a href="#Rect">Rect</a>
members, using(.

### Return Value

rounded <a href="SkIRect_Reference#IRect">IRect</a>

### Example

<div><fiddle-embed name="ef7ae1dd522c235b0afe41b55a624f46">

#### Example Output

~~~~
round: 31, 51, 41, 61
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_roundOut">roundOut</a> <a href="#SkRect_roundIn">roundIn</a> <a href="undocumented#SkScalarRoundToInt">SkScalarRoundToInt</a>

---

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
SkIRect roundOut() const
</pre>

Sets <a href="SkIRect_Reference#IRect">IRect</a> by discarding the fractional portion of <a href="#SkRect_fLeft">fLeft</a> and <a href="#SkRect_fTop">fTop</a>; and
rounding up <a href="#SkRect_fRight">fRight</a> and FBottom, using(.

### Return Value

rounded <a href="SkIRect_Reference#IRect">IRect</a>

### Example

<div><fiddle-embed name="05f0f65ae148f192656cd87df90f1d57">

#### Example Output

~~~~
round: 30, 50, 41, 61
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_round">round</a> <a href="#SkRect_roundIn">roundIn</a> <a href="undocumented#SkScalarRoundToInt">SkScalarRoundToInt</a>

---

<a name="SkRect_sort"></a>
## sort

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void sort()
</pre>

Swaps <a href="#SkRect_fLeft">fLeft</a> and <a href="#SkRect_fRight">fRight</a> if <a href="#SkRect_fLeft">fLeft</a> is greater than <a href="#SkRect_fRight">fRight</a>; and swaps
<a href="#SkRect_fTop">fTop</a> and <a href="#SkRect_fBottom">fBottom</a> if <a href="#SkRect_fTop">fTop</a> is greater than <a href="#SkRect_fBottom">fBottom</a>. Result may be empty;
and <a href="#SkRect_width">width</a> and <a href="#SkRect_height">height</a> will be zero or positive.

### Example

<div><fiddle-embed name="e624fe398e3d770b573c09fc74c0c400">

#### Example Output

~~~~
rect: 30.5, 50.5, 20.5, 10.5
sorted: 20.5, 10.5, 30.5, 50.5
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_makeSorted">makeSorted</a> <a href="#SkIRect_sort">SkIRect::sort</a>

---

<a name="SkRect_makeSorted"></a>
## makeSorted

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
SkRect makeSorted() const
</pre>

Returns <a href="#Rect">Rect</a> with <a href="#SkRect_fLeft">fLeft</a> and <a href="#SkRect_fRight">fRight</a> swapped if <a href="#SkRect_fLeft">fLeft</a> is greater than <a href="#SkRect_fRight">fRight</a>; and
with <a href="#SkRect_fTop">fTop</a> and <a href="#SkRect_fBottom">fBottom</a> swapped if <a href="#SkRect_fTop">fTop</a> is greater than <a href="#SkRect_fBottom">fBottom</a>. Result may be empty;
and <a href="#SkRect_width">width</a> and <a href="#SkRect_height">height</a> will be zero or positive.

### Return Value

sorted <a href="#Rect">Rect</a>

### Example

<div><fiddle-embed name="f59567042b87f6b26f9bfeeb04468032">

#### Example Output

~~~~
rect: 30.5, 50.5, 20.5, 10.5
sorted: 20.5, 10.5, 30.5, 50.5
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_sort">sort</a> <a href="#SkIRect_makeSorted">SkIRect::makeSorted</a>

---

<a name="SkRect_asScalars"></a>
## asScalars

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
const SkScalar* asScalars() const
</pre>

Returns pointer to first <a href="#Scalar">Scalar</a> in <a href="#Rect">Rect</a>, to treat it as an array with four
entries.

### Return Value

pointer to <a href="#SkRect_fLeft">fLeft</a>

### Example

<div><fiddle-embed name="e1ea5f949d80276f3637931eae93a07c">

#### Example Output

~~~~
rect.asScalars() == &rect.fLeft
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_toQuad">toQuad</a>

---

<a name="SkRect_dump"></a>
## dump

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void dump(bool asHex) const
</pre>

Writes text representation of <a href="#Rect">Rect</a> to standard output. Set <a href="#SkRect_dump_asHex">asHex</a> to true to 
generate exact binary representations of floating point numbers.

### Parameters

<table>  <tr>    <td><a name="SkRect_dump_asHex"> <code><strong>asHex </strong></code> </a></td> <td>
true if <a href="undocumented#SkScalar">SkScalar</a> values are written as hexadecimal</td>
  </tr>
</table>

### Example

<div><fiddle-embed name="cea049ffff702a5923da41fe0ae0763b">

#### Example Output

~~~~
SkRect::MakeLTRB(20, 30, 40, 50);
SkRect::MakeLTRB(SkBits2Float(0x41a00000), /* 20.000000 */
SkBits2Float(0x41f00000), /* 30.000000 */
SkBits2Float(0x42200000), /* 40.000000 */
SkBits2Float(0x42480000)  /* 50.000000 */);
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_dumpHex">dumpHex</a>

---

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void dump() const
</pre>

Writes text representation of <a href="#Rect">Rect</a> to standard output. The representation may be
directly compiled as <a href="undocumented#C">C</a>++ code. Floating point values are written
with limited precision; it may not be possible to reconstruct original <a href="#Rect">Rect</a>
from output.

### Example

<div><fiddle-embed name="9fb76971b1a104a2a59816e0392267a7">

#### Example Output

~~~~
SkRect::MakeLTRB(0.857143f, 0.666667f, 2.6f, 7);
rect is not equal to copy
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_dumpHex">dumpHex</a>

---

<a name="SkRect_dumpHex"></a>
## dumpHex

<pre style="padding: 1em 1em 1em 1em;width: 62.5em; background-color: #f0f0f0">
void dumpHex() const
</pre>

Writes text representation of <a href="#Rect">Rect</a> to standard output. The representation may be
directly compiled as <a href="undocumented#C">C</a>++ code. Floating point values are written
in hexadecimal to preserve their exact bit pattern. The output reconstructs the
original <a href="#Rect">Rect</a>.

Use instead of <a href="#SkRect_dump_2">dump</a> when submittingbug reports against <a href="undocumented#Skia">Skia</a>.

### Example

<div><fiddle-embed name="824b5a3fcfd46a7e1c5f9e3c16e6bb39">

#### Example Output

~~~~
SkRect::MakeLTRB(SkBits2Float(0x3f5b6db7), /* 0.857143 */
SkBits2Float(0x3f2aaaab), /* 0.666667 */
SkBits2Float(0x40266666), /* 2.600000 */
SkBits2Float(0x40e00000)  /* 7.000000 */);
rect is equal to copy
~~~~

</fiddle-embed></div>

### See Also

<a href="#SkRect_dump">dump</a>

---

