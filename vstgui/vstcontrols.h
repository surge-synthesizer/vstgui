//-----------------------------------------------------------------------------
// VST Plug-Ins SDK
// VSTGUI: Graphical User Interface Framework for VST plugins : 
// Standard Control Objects
//
// Version 3.5       $Date: 2007-02-02 15:35:16 $
//
//-----------------------------------------------------------------------------
// VSTGUI LICENSE
// � 2004, Steinberg Media Technologies, All Rights Reserved
//-----------------------------------------------------------------------------
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
//   * Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation 
//     and/or other materials provided with the distribution.
//   * Neither the name of the Steinberg Media Technologies nor the names of its
//     contributors may be used to endorse or promote products derived from this 
//     software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A  PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE  OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#ifndef __vstcontrols__
#define __vstcontrols__

#ifndef __vstgui__
#include "vstgui.h"
#endif

//------------------
// defines
//------------------
#ifndef kPI
#define kPI    3.14159265358979323846
#endif

#ifndef k2PI
#define k2PI   6.28318530717958647692
#endif

#ifndef kPI_2
#define kPI_2  1.57079632679489661923f
#endif

#ifndef kPI_4
#define kPI_4  0.78539816339744830962
#endif

#ifndef kE
#define kE     2.7182818284590452354
#endif

#ifndef kLN2
#define kLN2   0.69314718055994530942
#endif

#ifndef kSQRT2
#define kSQRT2 1.41421356237309504880
#endif

//------------------
// CControlEnum type
//------------------
enum CControlEnum
{
	kHorizontal			= 1 << 0,
	kVertical			= 1 << 1,
	kShadowText			= 1 << 2,
	kLeft				= 1 << 3,
	kRight				= 1 << 4,
	kTop				= 1 << 5,
	kBottom				= 1 << 6,
	k3DIn				= 1 << 7,
	k3DOut				= 1 << 8,
	kPopupStyle			= 1 << 9,
	kCheckStyle			= 1 << 10,
	kMultipleCheckStyle,
	kNoTextStyle		= 1 << 11,
	kNoDrawStyle		= 1 << 12,
	kDoubleClickStyle	= 1 << 13,
	kNoFrame			= 1 << 14
};

//---------------------------
// Some defines for Menu item
//---------------------------
#define kMenuTitle     "-T"
#define kMenuSeparator "-"
#define kMenuDisable   "-G"
#define kMenuSubMenu   "-M"

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class CControlListener
{
public:
	#if USE_NAMESPACE
	virtual void valueChanged (VSTGUI::CControl* pControl) = 0;
	virtual long controlModifierClicked (VSTGUI::CControl* pControl, long button) { return 0; }	///< return 1 if you want the control to not handle it, otherwise 0
	virtual void controlBeginEdit (VSTGUI::CControl* pControl) {}
	virtual void controlEndEdit (VSTGUI::CControl* pControl) {}
	#else
	virtual void valueChanged (CControl* pControl) = 0;
	virtual long controlModifierClicked (CControl* pControl, long button) { return 0; }	///< return 1 if you want the control to not handle it, otherwise 0
	virtual void controlBeginEdit (CControl* pControl) {}
	virtual void controlEndEdit (CControl* pControl) {}
	#endif
};

//-----------------------------------------------------------------------------
BEGIN_NAMESPACE_VSTGUI
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CControl Declaration
//! \brief base class of all VSTGUI controls
/// \nosubgrouping
//-----------------------------------------------------------------------------
class CControl : public CView
{
public:
	//-----------------------------------------------------------------------------
	/// \name Constructor
	//-----------------------------------------------------------------------------
	//@{
	CControl (const CRect& size, CControlListener* listener = 0, long tag = 0, CBitmap* pBackground = 0);
	CControl (const CControl& c);
	//@}

	virtual ~CControl ();

	virtual void  doIdleStuff () { if (pParentFrame) pParentFrame->doIdleStuff (); }

	//-----------------------------------------------------------------------------
	/// \name Value Methods
	//-----------------------------------------------------------------------------
	//@{
	virtual void  setValue (float val) { value = val; }
	virtual float getValue () const { return value; };

	virtual void  setMin (float val) { vmin = val; }
	virtual float getMin () const { return vmin; }
	virtual void  setMax (float val) { vmax = val; }
	virtual float getMax () const { return vmax; }

	virtual void  setOldValue (float val) { oldValue = val; }
	virtual	float getOldValue (void) const { return oldValue; }
	virtual void  setDefaultValue (float val) { defaultValue = val; }
	virtual	float getDefaultValue (void) const { return defaultValue; }

	virtual void bounceValue ();
	virtual bool checkDefaultValue (long button);
	//@}

	//-----------------------------------------------------------------------------
	/// \name Editing Methods
	//-----------------------------------------------------------------------------
	//@{
	virtual void  setTag (long val) { tag = val; }
	virtual long  getTag () const { return tag; }

	virtual void beginEdit ();
	virtual void endEdit ();

	virtual CControlListener* getListener () const { return listener; }
	virtual void setListener (CControlListener* l) { listener = l; }
	//@}

	virtual void setBackOffset (CPoint& offset);
	virtual void copyBackOffset ();

	virtual void  setWheelInc (float val) { wheelInc = val; }
	virtual float getWheelInc () const { return wheelInc; }

	void draw (CDrawContext* pContext) = 0;
	bool  isDirty () const;
	void  setDirty (const bool val = true);

	VSTGUI_DEPRECATED(bool isDoubleClick ();) ///< \deprecated

	CLASS_METHODS_VIRTUAL(CControl, CView)

protected:
	CControlListener* listener;
	long  tag;
	float oldValue;
	float defaultValue;
	float value;
	float vmin;
	float vmax;
	float wheelInc;

	long lastTicks;
	long delta;

	CPoint	backOffset;
};

//-----------------------------------------------------------------------------
// IMultiBitmap Declaration
//! \brief interface for controls with sub images
/// \nosubgrouping
//-----------------------------------------------------------------------------
class IMultiBitmapControl
{
public:
	virtual void setHeightOfOneImage (const CCoord& height) { heightOfOneImage = height; }
	virtual CCoord getHeightOfOneImage () const { return heightOfOneImage; }

	virtual void autoComputeHeightOfOneImage ();
protected:
	CCoord heightOfOneImage;
};

//-----------------------------------------------------------------------------
// COnOffButton Declaration
//! \brief a button control with 2 states
/// \nosubgrouping
//-----------------------------------------------------------------------------
class COnOffButton : public CControl
{
public:
	//-----------------------------------------------------------------------------
	/// \name Constructor
	//-----------------------------------------------------------------------------
	//@{
	COnOffButton (const CRect& size, CControlListener* listener, long tag, CBitmap* background, long style = kPreListenerUpdate);
	COnOffButton (const COnOffButton& onOffButton);
	//@}
	
	virtual ~COnOffButton ();

	virtual void draw (CDrawContext*);
	VSTGUI_DEPRECATED(virtual void mouse (CDrawContext* pContext, CPoint& where, long button = -1);)
	virtual CMouseEventResult onMouseDown (CPoint& where, const long& buttons);

	virtual long getStyle () const { return style; }
	virtual void setStyle (long newStyle) { style = newStyle; }

	enum {
		kPreListenerUpdate,			///< listener will be called after doIdleStuff was called
		kPostListenerUpdate,		///< listener will be called before doIdleStuff is called
	};

	CLASS_METHODS(COnOffButton, CControl)
protected:
	long style;
};


//-----------------------------------------------------------------------------
// CParamDisplay Declaration
//! \brief a parameter display control
/// \nosubgrouping
//-----------------------------------------------------------------------------
class CParamDisplay : public CControl
{
public:
	//-----------------------------------------------------------------------------
	/// \name Constructor
	//-----------------------------------------------------------------------------
	//@{
	CParamDisplay (const CRect& size, CBitmap* background = 0, const long style = 0);
	CParamDisplay (const CParamDisplay& paramDisplay);
	//@}

	virtual ~CParamDisplay ();
	
	virtual void setFont (CFontRef fontID);
	const CFontRef getFont () const { return fontID; }

	virtual void setFontColor (CColor color);
	CColor getFontColor () const { return fontColor; }

	virtual void setBackColor (CColor color);
	CColor getBackColor () const { return backColor; }

	virtual void setFrameColor (CColor color);
	CColor getFrameColor () const { return frameColor; }

	virtual void setShadowColor (CColor color);
	CColor getShadowColor () const { return shadowColor; }

	virtual void setAntialias (bool state) { bAntialias = state; }
	bool getAntialias () const { return bAntialias; }

	virtual void setHoriAlign (CHoriTxtAlign hAlign);
	CHoriTxtAlign getHoriAlign () const { return horiTxtAlign; }

	virtual void setStringConvert (void (*convert) (float value, char* string));
	virtual void setStringConvert (void (*convert) (float value, char* string, void* userDta),
									void* userData);
	virtual void setString2FloatConvert (void (*convert) (char* string, float& output));

	virtual void setStyle (long val);
	long getStyle () const { return style; }

	virtual void setTxtFace (CTxtFace val);
	CTxtFace getTxtFace () const { return txtFace; }

	virtual void draw (CDrawContext* pContext);

	virtual void setTextTransparency (bool val) { bTextTransparencyEnabled = val; }
	bool getTextTransparency () const { return bTextTransparencyEnabled; }

	CLASS_METHODS(CParamDisplay, CControl)

protected:
	void drawText (CDrawContext* pContext, char* string, CBitmap* newBack = 0);

	void (*stringConvert) (float value, char* string);
	void (*stringConvert2) (float value, char* string, void* userData);
	void (*string2FloatConvert) (char* string, float& output);
	void* userData;

	CHoriTxtAlign horiTxtAlign;
	long    style;

	CFontRef   fontID;
	CTxtFace txtFace;
	CColor  fontColor;
	CColor  backColor;
	CColor  frameColor;
	CColor  shadowColor;
	bool    bTextTransparencyEnabled;
	bool	bAntialias;
};


//-----------------------------------------------------------------------------
// CLabel Declaration
//! \brief a text label
/// \nosubgrouping
//-----------------------------------------------------------------------------
class CTextLabel : public CParamDisplay
{
public:
	//-----------------------------------------------------------------------------
	/// \name Constructor
	//-----------------------------------------------------------------------------
	//@{
	CTextLabel (const CRect& size, const char* txt = 0, CBitmap* background = 0, const long style = 0);
	CTextLabel (const CTextLabel& textLabel);
	//@}
	
	~CTextLabel ();
	
	virtual void setText (const char* txt);
	virtual const char* getText () const;
	
	virtual	void draw (CDrawContext* pContext);

	CLASS_METHODS(CTextLabel, CParamDisplay)

protected:
	void freeText ();
	char* text;
};

//-----------------------------------------------------------------------------
// CTextEdit Declaration
//! \brief a text edit control
/// \nosubgrouping
//-----------------------------------------------------------------------------
class CTextEdit : public CParamDisplay
{
public:
	//-----------------------------------------------------------------------------
	/// \name Constructor
	//-----------------------------------------------------------------------------
	//@{
	CTextEdit (const CRect& size, CControlListener* listener, long tag, const char* txt = 0, CBitmap* background = 0, const long style = 0);
	CTextEdit (const CTextEdit& textEdit);
	//@}

	virtual ~CTextEdit ();

	virtual void setText (char* txt);
	virtual void getText (char* txt) const;
	virtual const char* getText () const { return text; }

	virtual	void draw (CDrawContext* pContext);
	VSTGUI_DEPRECATED(virtual void mouse (CDrawContext* pContext, CPoint& where, long button = -1);)
	virtual CMouseEventResult onMouseDown (CPoint& where, const long& buttons);

	virtual void setTextEditConvert (void (*editConvert) (char* input, char* string));
	virtual void setTextEditConvert (void (*editConvert2) (char* input, char* string,
										void* userDta), void* userData);

	virtual	void takeFocus ();
	virtual	void looseFocus ();

	virtual void setViewSize (CRect& newSize, bool invalid = true);
	virtual void parentSizeChanged ();

	void* platformFontColor;
	void* platformControl;
	bool bWasReturnPressed;

	CLASS_METHODS(CTextEdit, CParamDisplay)

protected:
	void* platformFont;
	char text[256];

	void (*editConvert) (char* input, char* string);
	void (*editConvert2) (char* input, char* string, void* userData);
};


//-----------------------------------------------------------------------------
// COptionMenuScheme Declaration
/// \nosubgrouping
//-----------------------------------------------------------------------------
class COptionMenuScheme : public CBaseObject
{
public:
	//-----------------------------------------------------------------------------
	/// \name Constructor
	//-----------------------------------------------------------------------------
	//@{
	COptionMenuScheme ();
	//@}
	
	virtual ~COptionMenuScheme ();

	enum { kChecked = 0x01, kDisabled = 0x02, kSelected = 0x04, kSubMenu = 0x08, kTitle = 0x10 };

	virtual void getItemSize (const char* text, CDrawContext* pContext, CPoint& size);
	virtual void drawItem (const char* text, long itemId, long state, CDrawContext* pContext, const CRect& rect);	

	void setColors (CColor back, CColor select, CColor text, CColor htext, CColor dtext)
	{ backgroundColor = back; selectionColor = select; textColor = text;
	hiliteTextColor = htext; disableTextColor = dtext; }
	
	void setFont (CFontRef f) { if (font) font->forget (); font = f; if (font) font->remember (); }
protected:

	CColor backgroundColor;
	CColor selectionColor;
	CColor textColor;
	CColor hiliteTextColor;
	CColor disableTextColor;
	CFontRef font;

	virtual void drawItemBack (CDrawContext* pContext, const CRect& rect, bool hilite);

	#if MAC_ENABLE_MENU_SCHEME
	static pascal OSStatus eventHandler (EventHandlerCallRef inCallRef, EventRef inEvent, void* inUserData);
	void registerWithToolbox ();
	void unregisterWithToolbox ();
	#endif
};

//-----------------------------------------------------------------------------
extern COptionMenuScheme* gOptionMenuScheme;

//-----------------------------------------------------------------------------
// COptionMenu Declaration
//! \brief a popup menu control
/// \nosubgrouping
//-----------------------------------------------------------------------------
class COptionMenu : public CParamDisplay
{
public:
	//-----------------------------------------------------------------------------
	/// \name Constructor
	//-----------------------------------------------------------------------------
	//@{
	COptionMenu (const CRect& size, CControlListener* listener, long tag, CBitmap* background = 0, CBitmap* bgWhenClick = 0, const long style = 0);
	COptionMenu (const COptionMenu& menu);
	//@}

	virtual ~COptionMenu ();

	enum { MAX_ENTRY = 1024 };

	virtual void setValue (float val);
	virtual bool addEntry (COptionMenu *subMenu, const char *txt);
	virtual	bool addEntry (const char *txt, long index = -1);
	virtual	long getCurrent (char *txt = 0, bool countSeparator = true) const;
	virtual	bool setCurrent (long index, bool countSeparator = true);
	virtual	bool getEntry (long index, char* txt) const;
	virtual	bool setEntry (long index, char* txt);
	virtual	bool removeEntry (long index);
	virtual	bool removeAllEntry ();
	virtual long getNbEntries () const { return nbEntries; }
	virtual long getIndex (char* txt) const;

	virtual bool checkEntry (long index, bool state);
	virtual bool checkEntryAlone (long index);
	virtual bool isCheckEntry (long index) const;

	virtual	void draw (CDrawContext* pContext);
	VSTGUI_DEPRECATED (virtual void mouse (CDrawContext* pContext, CPoint& where, long button = -1);)
	virtual CMouseEventResult onMouseDown (CPoint& where, const long& buttons);

	virtual	void takeFocus ();
	virtual	void looseFocus ();

	virtual void setNbItemsPerColumn (long val) { nbItemsPerColumn = val; }
	virtual long getNbItemsPerColumn () const { return nbItemsPerColumn; }

#if MAC
	short   getMenuID () const { return menuID; }
#endif

	long getLastResult () const { return lastResult; }
	COptionMenu* getLastItemMenu (long& idxInMenu) const;

	void setScheme (COptionMenuScheme* s) { scheme = s; }
	virtual COptionMenuScheme* getScheme () const { return scheme; }

	virtual void setPrefixNumbers (long preCount);

	COptionMenu* getSubMenu (long idx) const;

	CLASS_METHODS(COptionMenu, CParamDisplay)

protected:
	COptionMenu* getItemMenu (long idx, long& idxInMenu, long& offsetIdx);
	void  removeItems ();
	void* appendItems (long& offsetIdx);

	void* platformControl;

	bool  allocateMenu (long nb);
	bool  allocateSubMenu (long nb);

	char** entry;
	COptionMenu** submenuEntry;
	bool* check;

#if MAC
	short   menuID;
#endif

	long     nbEntries;
	long     nbSubMenus;
	long     currentIndex;
	CBitmap* bgWhenClick;
	long     lastButton;
	long     nbItemsPerColumn;
	long     nbAllocated;
	long     nbSubMenuAllocated;
	long	 lastResult;
	long	 prefixNumbers;
	COptionMenu* lastMenu;
	COptionMenuScheme* scheme;
};


//-----------------------------------------------------------------------------
// CKnob Declaration
//! \brief a knob control
/// \nosubgrouping
//-----------------------------------------------------------------------------
class CKnob : public CControl
{
public:
	//-----------------------------------------------------------------------------
	/// \name Constructor
	//-----------------------------------------------------------------------------
	//@{
	CKnob (const CRect& size, CControlListener* listener, long tag, CBitmap* background, CBitmap* handle, const CPoint& offset = CPoint (0, 0));
	CKnob (const CKnob& knob);
	//@}

	virtual ~CKnob ();

	virtual void draw (CDrawContext* pContext);
	VSTGUI_DEPRECATED(virtual void mouse (CDrawContext* pContext, CPoint& where, long button = -1);)
	virtual bool onWheel (const CPoint& where, const float& distance, const long& buttons);
	virtual long onKeyDown (VstKeyCode& keyCode);

	virtual CMouseEventResult onMouseDown (CPoint& where, const long& buttons);
	virtual CMouseEventResult onMouseUp (CPoint& where, const long& buttons);
	virtual CMouseEventResult onMouseMoved (CPoint& where, const long& buttons);

	virtual void drawHandle (CDrawContext* pContext);

	virtual void  setStartAngle (float val);
	virtual float getStartAngle () const { return startAngle; }

	virtual void  setRangeAngle (float val);
	virtual float getRangeAngle () const { return rangeAngle; }

	virtual void  valueToPoint (CPoint& point) const;
	virtual float valueFromPoint (CPoint& point) const;

	virtual long getInsetValue () const { return inset; }
	virtual void setInsetValue (long val) { inset = val; }

	virtual CColor getColorShadowHandle () const { return colorShadowHandle; }
	virtual void setColorShadowHandle (CColor color);

	virtual CColor getColorHandle () const { return colorHandle; }
	virtual void setColorHandle (CColor color);

	virtual CBitmap* getHandleBitmap () const { return pHandle; }
	virtual void setHandleBitmap (CBitmap* bitmap);

	virtual void  setZoomFactor (float val) { zoomFactor = val; }
	virtual float getZoomFactor () const { return zoomFactor; }

	CLASS_METHODS(CKnob, CControl)

protected:
	void compute ();

	CPoint offset;
	
	CColor   colorHandle, colorShadowHandle;

	CBitmap* pHandle;
	long     inset;
	float    startAngle, rangeAngle, halfAngle;
	float    aCoef, bCoef;
	float    radius;
	float    zoomFactor;

private:
	CPoint firstPoint;
	CPoint lastPoint;
	float  startValue;
	float  fEntryState;
	float  range;
	float  coef;
	long   oldButton;
	bool   modeLinear;
	
};

//-----------------------------------------------------------------------------
// CAnimKnob Declaration
//! \brief a bitmap knob control
/// \nosubgrouping
//-----------------------------------------------------------------------------
class CAnimKnob : public CKnob, public IMultiBitmapControl
{
public:
	//-----------------------------------------------------------------------------
	/// \name Constructor
	//-----------------------------------------------------------------------------
	//@{
	CAnimKnob (const CRect& size, CControlListener* listener, long tag, CBitmap* background, const CPoint& offset = CPoint (0, 0));
	CAnimKnob (const CRect& size, CControlListener* listener, long tag, long subPixmaps, CCoord heightOfOneImage, CBitmap* background, const CPoint& offset = CPoint (0, 0));
	CAnimKnob (const CAnimKnob& knob);
	//@}
	
	virtual ~CAnimKnob ();

	virtual bool isDirty () const;

	virtual void draw (CDrawContext* pContext);

	void setInverseBitmap (bool val) { bInverseBitmap = val; }

	void setHeightOfOneImage (const CCoord& height);

	CLASS_METHODS(CAnimKnob, CKnob)

protected:
	long	subPixmaps;
	bool	bInverseBitmap;
	CPoint	lastDrawnPoint;
};

//-----------------------------------------------------------------------------
// CVerticalSwitch Declaration
//! \brief a vertical switch control
/// \nosubgrouping
//-----------------------------------------------------------------------------
class CVerticalSwitch : public CControl, public IMultiBitmapControl
{
public:
	//-----------------------------------------------------------------------------
	/// \name Constructor
	//-----------------------------------------------------------------------------
	//@{
	CVerticalSwitch (const CRect& size, CControlListener* listener, long tag, CBitmap* background, const CPoint& offset = CPoint (0, 0));
	CVerticalSwitch (const CRect& size, CControlListener* listener, long tag, long subPixmaps, CCoord heightOfOneImage, long iMaxPositions, CBitmap* background, const CPoint& offset = CPoint (0, 0));
	CVerticalSwitch (const CVerticalSwitch& vswitch);
	//@}

	virtual ~CVerticalSwitch ();

	virtual void draw (CDrawContext*);
	VSTGUI_DEPRECATED(virtual void mouse (CDrawContext* pContext, CPoint& where, long button = -1);)

	virtual CMouseEventResult onMouseDown (CPoint& where, const long& buttons);
	virtual CMouseEventResult onMouseUp (CPoint& where, const long& buttons);
	virtual CMouseEventResult onMouseMoved (CPoint& where, const long& buttons);

	CLASS_METHODS(CVerticalSwitch, CControl)

protected:
	CPoint	offset;
	long	subPixmaps;
	long	iMaxPositions;

private:
	double coef;
};


//-----------------------------------------------------------------------------
// CHorizontalSwitch Declaration
//! \brief a horizontal switch control
/// \nosubgrouping
//-----------------------------------------------------------------------------
class CHorizontalSwitch : public CControl, public IMultiBitmapControl
{
public:
	//-----------------------------------------------------------------------------
	/// \name Constructor
	//-----------------------------------------------------------------------------
	//@{
	CHorizontalSwitch (const CRect& size, CControlListener* listener, long tag, CBitmap* background, const CPoint& offset = CPoint (0, 0));
	CHorizontalSwitch (const CRect& size, CControlListener* listener, long tag, long subPixmaps, CCoord heightOfOneImage, long iMaxPositions, CBitmap* background, const CPoint& offset = CPoint (0, 0));
	CHorizontalSwitch (const CHorizontalSwitch& hswitch);
	//@}
	
	virtual	~CHorizontalSwitch ();

	virtual void draw (CDrawContext*);
	VSTGUI_DEPRECATED(virtual void mouse (CDrawContext* pContext, CPoint& where, long button = -1);)

	virtual CMouseEventResult onMouseDown (CPoint& where, const long& buttons);
	virtual CMouseEventResult onMouseUp (CPoint& where, const long& buttons);
	virtual CMouseEventResult onMouseMoved (CPoint& where, const long& buttons);

	CLASS_METHODS(CHorizontalSwitch, CControl)

protected:
	CPoint	offset;
	long	subPixmaps;
	long	iMaxPositions;

private:
	double coef;
};


//-----------------------------------------------------------------------------
// CRockerSwitch Declaration
//! \brief a switch control with 3 sub bitmaps
/// \nosubgrouping
//-----------------------------------------------------------------------------
class CRockerSwitch : public CControl, public IMultiBitmapControl
{
public:
	//-----------------------------------------------------------------------------
	/// \name Constructor
	//-----------------------------------------------------------------------------
	//@{
	CRockerSwitch (const CRect& size, CControlListener* listener, long tag, CBitmap* background, const CPoint& offset = CPoint (0, 0), const long style = kHorizontal);
	CRockerSwitch (const CRect& size, CControlListener* listener, long tag, CCoord heightOfOneImage, CBitmap* background, const CPoint& offset = CPoint (0, 0), const long style = kHorizontal);
	CRockerSwitch (const CRockerSwitch& rswitch);
	//@}

	virtual ~CRockerSwitch ();

	virtual void draw (CDrawContext*);
	VSTGUI_DEPRECATED(virtual void mouse (CDrawContext* pContext, CPoint& where, long button = -1);)
	virtual bool onWheel (const CPoint& where, const float& distance, const long& buttons);

	virtual CMouseEventResult onMouseDown (CPoint& where, const long& buttons);
	virtual CMouseEventResult onMouseUp (CPoint& where, const long& buttons);
	virtual CMouseEventResult onMouseMoved (CPoint& where, const long& buttons);

	CLASS_METHODS(CRockerSwitch, CControl)

protected:
	CPoint	offset;
	long	style;

private:
	float fEntryState;
};


//-----------------------------------------------------------------------------
// CMovieBitmap Declaration
//! \brief a bitmap control that displays different bitmaps according to its current value
/// \nosubgrouping
//-----------------------------------------------------------------------------
class CMovieBitmap : public CControl, public IMultiBitmapControl
{
public:
	//-----------------------------------------------------------------------------
	/// \name Constructor
	//-----------------------------------------------------------------------------
	//@{
	CMovieBitmap (const CRect& size, CControlListener* listener, long tag, CBitmap* background, const CPoint& offset = CPoint (0, 0));
	CMovieBitmap (const CRect& size, CControlListener* listener, long tag, long subPixmaps, CCoord heightOfOneImage, CBitmap* background, const CPoint& offset = CPoint (0, 0));
	CMovieBitmap (const CMovieBitmap& movieBitmap);
	//@}

	virtual	~CMovieBitmap ();

	virtual void draw (CDrawContext*);

	CLASS_METHODS(CMovieBitmap, CControl)

protected:
	CPoint	offset;
	long	subPixmaps;
};


//-----------------------------------------------------------------------------
// CMovieButton Declaration
//! \brief a bi-states button with 2 subbitmaps
/// \nosubgrouping
//-----------------------------------------------------------------------------
class CMovieButton : public CControl, public IMultiBitmapControl
{
public:
	//-----------------------------------------------------------------------------
	/// \name Constructor
	//-----------------------------------------------------------------------------
	//@{
	CMovieButton (const CRect& size, CControlListener* listener, long tag, CBitmap* background, const CPoint& offset = CPoint (0, 0));
	CMovieButton (const CRect& size, CControlListener* listener, long tag, CCoord heightOfOneImage, CBitmap* background, const CPoint& offset = CPoint (0, 0));
	CMovieButton (const CMovieButton& movieButton);
	//@}

	virtual ~CMovieButton ();	

	virtual void draw (CDrawContext*);
	VSTGUI_DEPRECATED(virtual void mouse (CDrawContext* pContext, CPoint& where, long button = -1);)

	virtual CMouseEventResult onMouseDown (CPoint& where, const long& buttons);
	virtual CMouseEventResult onMouseUp (CPoint& where, const long& buttons);
	virtual CMouseEventResult onMouseMoved (CPoint& where, const long& buttons);

	CLASS_METHODS(CMovieButton, CControl)

protected:
	CPoint   offset;
	float    buttonState;

private:
	float    fEntryState;
};


//-----------------------------------------------------------------------------
// CAutoAnimation Declaration
//!
/// \nosubgrouping
//-----------------------------------------------------------------------------
class CAutoAnimation : public CControl, public IMultiBitmapControl
{
public:
	//-----------------------------------------------------------------------------
	/// \name Constructor
	//-----------------------------------------------------------------------------
	//@{
	CAutoAnimation (const CRect& size, CControlListener* listener, long tag, CBitmap* background, const CPoint& offset = CPoint (0, 0));
	CAutoAnimation (const CRect& size, CControlListener* listener, long tag, long subPixmaps, CCoord heightOfOneImage, CBitmap* background, const CPoint& offset = CPoint (0, 0));
	CAutoAnimation (const CAutoAnimation& autoAnimation);
	//@}

	virtual ~CAutoAnimation ();

	virtual void draw (CDrawContext*);
	VSTGUI_DEPRECATED(virtual void mouse (CDrawContext* pContext, CPoint& where, long button = -1);)
	virtual CMouseEventResult onMouseDown (CPoint& where, const long& buttons);

	virtual void openWindow (void);
	virtual void closeWindow (void);

	virtual void nextPixmap (void);
	virtual void previousPixmap (void);

	bool    isWindowOpened () const { return bWindowOpened; }

	CLASS_METHODS(CAutoAnimation, CControl)

protected:
	CPoint	offset;

	long	subPixmaps;
	CCoord	totalHeightOfBitmap;

	bool	bWindowOpened;
};


//-----------------------------------------------------------------------------
// CSlider Declaration
//! \brief a slider control
/// \nosubgrouping
//-----------------------------------------------------------------------------
class CSlider : public CControl
{
public:
	//-----------------------------------------------------------------------------
	/// \name Constructor
	//-----------------------------------------------------------------------------
	//@{
	CSlider (const CRect& size, CControlListener* listener, long tag, long iMinPos, long iMaxPos, CBitmap* handle, CBitmap* background, const CPoint& offset = CPoint (0, 0), const long style = kLeft|kHorizontal);
	CSlider (const CRect& rect, CControlListener* listener, long tag, const CPoint& offsetHandle, long rangeHandle, CBitmap* handle, CBitmap* background, const CPoint& offset = CPoint (0, 0), const long style = kLeft|kHorizontal);
	CSlider (const CSlider& slider);
	//@}

	virtual ~CSlider ();
  
	virtual void draw (CDrawContext*);
	VSTGUI_DEPRECATED(virtual void mouse (CDrawContext* pContext, CPoint& where, long button = -1);)

	virtual CMouseEventResult onMouseDown (CPoint& where, const long& buttons);
	virtual CMouseEventResult onMouseUp (CPoint& where, const long& buttons);
	virtual CMouseEventResult onMouseMoved (CPoint& where, const long& buttons);

	virtual bool onWheel (const CPoint& where, const float& distance, const long& buttons);
	virtual long onKeyDown (VstKeyCode& keyCode);

	virtual void setDrawTransparentHandle (bool val) { bDrawTransparentEnabled = val; }
	virtual void setFreeClick (bool val) { bFreeClick = val; }
	virtual bool getFreeClick () const { return bFreeClick; }
	virtual void setOffsetHandle (const CPoint& val);
	virtual CPoint getOffsetHandle () const { return offsetHandle; }
	virtual void setOffset (const CPoint& val) { offset = val; }
	virtual CPoint getOffset () const { return offset; }

	virtual long getStyle () const { return style; }

	virtual void     setHandle (CBitmap* pHandle);
	virtual CBitmap* getHandle () const { return pHandle; }

	virtual void  setZoomFactor (float val) { zoomFactor = val; }
	virtual float getZoomFactor () const { return zoomFactor; }

	CLASS_METHODS(CSlider, CControl)

protected:
	CPoint   offset; 
	CPoint   offsetHandle;

	CBitmap* pHandle;

	long	style;

	CCoord	widthOfSlider;
	CCoord	heightOfSlider;
	CCoord	rangeHandle;
	CCoord	minTmp;
	CCoord	maxTmp;
	CCoord	minPos;
	CCoord	widthControl;
	CCoord	heightControl;
	float	zoomFactor;

	bool     bDrawTransparentEnabled;
	bool     bFreeClick;

private:
	CCoord   delta;
	float    oldVal;
	long     oldButton; 
};

//-----------------------------------------------------------------------------
// CVerticalSlider Declaration
//! \brief a vertical slider control
/// \nosubgrouping
//-----------------------------------------------------------------------------
class CVerticalSlider : public CSlider
{
public:
	//-----------------------------------------------------------------------------
	/// \name Constructor
	//-----------------------------------------------------------------------------
	//@{
	CVerticalSlider (const CRect& size, CControlListener* listener, long tag, long iMinPos, long iMaxPos, CBitmap* handle, CBitmap* background, const CPoint& offset = CPoint (0, 0), const long style = kBottom);
	CVerticalSlider (const CRect& rect, CControlListener* listener, long tag, const CPoint& offsetHandle, long rangeHandle, CBitmap* handle, CBitmap* background, const CPoint& offset = CPoint (0, 0), const long style = kBottom);
	CVerticalSlider (const CVerticalSlider& slider);
	//@}
};

//-----------------------------------------------------------------------------
// CHorizontalSlider Declaration
//! \brief a horizontal slider control
/// \nosubgrouping
//-----------------------------------------------------------------------------
class CHorizontalSlider : public CSlider
{
public:
	//-----------------------------------------------------------------------------
	/// \name Constructor
	//-----------------------------------------------------------------------------
	//@{
	CHorizontalSlider (const CRect& size, CControlListener* listener, long tag, long iMinPos, long iMaxPos, CBitmap* handle, CBitmap* background, const CPoint& offset = CPoint (0, 0), const long style = kRight);
	CHorizontalSlider (const CRect& rect, CControlListener* listener, long tag, const CPoint& offsetHandle, long rangeHandle, CBitmap* handle, CBitmap* background, const CPoint& offset = CPoint (0, 0), const long style = kRight);
	CHorizontalSlider (const CHorizontalSlider& slider);
	//@}
};


//-----------------------------------------------------------------------------
// CSpecialDigit Declaration
//! \brief special display with custom numbers (0...9)
/// \nosubgrouping
//-----------------------------------------------------------------------------
class CSpecialDigit : public CControl
{
public:
	//-----------------------------------------------------------------------------
	/// \name Constructor
	//-----------------------------------------------------------------------------
	//@{
	CSpecialDigit (const CRect& size, CControlListener* listener, long tag, long dwPos, long iNumbers, long* xpos, long* ypos, long width, long height, CBitmap* background);
	CSpecialDigit (const CSpecialDigit& digit);
	//@}

	virtual ~CSpecialDigit ();
	
	virtual void  draw (CDrawContext*);

	virtual float getNormValue (void) const;

	CLASS_METHODS(CSpecialDigit, CControl)

protected:
	long     iNumbers;
	long     xpos[7];
	long     ypos[7];
	long     width;
	long     height;
};


//-----------------------------------------------------------------------------
// CKickButton Declaration
//!
/// \nosubgrouping
//-----------------------------------------------------------------------------
class CKickButton : public CControl, public IMultiBitmapControl
{
public:
	//-----------------------------------------------------------------------------
	/// \name Constructor
	//-----------------------------------------------------------------------------
	//@{
	CKickButton (const CRect& size, CControlListener* listener, long tag, CBitmap* background, const CPoint& offset = CPoint (0, 0));
	CKickButton (const CRect& size, CControlListener* listener, long tag, CCoord heightOfOneImage, CBitmap* background, const CPoint& offset = CPoint (0, 0));
	CKickButton (const CKickButton& kickButton);
	//@}

	virtual ~CKickButton ();	

	virtual void draw (CDrawContext*);
	VSTGUI_DEPRECATED(virtual void mouse (CDrawContext* pContext, CPoint& where, long button = -1);)

	virtual CMouseEventResult onMouseDown (CPoint& where, const long& buttons);
	virtual CMouseEventResult onMouseUp (CPoint& where, const long& buttons);
	virtual CMouseEventResult onMouseMoved (CPoint& where, const long& buttons);

	CLASS_METHODS(CKickButton, CControl)

protected:
	CPoint	offset;

private:
	float   fEntryState;
};


//-----------------------------------------------------------------------------
// CSplashScreen Declaration
//!
/// \nosubgrouping
//-----------------------------------------------------------------------------
class CSplashScreen : public CControl, public CControlListener
{
public:
	//-----------------------------------------------------------------------------
	/// \name Constructor
	//-----------------------------------------------------------------------------
	//@{
	CSplashScreen (const CRect& size, CControlListener* listener, long tag, CBitmap* background, CRect& toDisplay, const CPoint& offset = CPoint (0, 0));
	CSplashScreen (const CRect& size, CControlListener* listener, long tag, CView* splashView);
	CSplashScreen (const CSplashScreen& splashScreen);
	//@}
	virtual ~CSplashScreen ();	
  
	virtual void draw (CDrawContext*);
	virtual bool hitTest (const CPoint& where, const long buttons = -1);
	VSTGUI_DEPRECATED(virtual void mouse (CDrawContext* pContext, CPoint& where, long button = -1);)
	virtual void unSplash ();

	virtual void setDisplayArea (const CRect& rect)  { toDisplay = rect; }				///< set the area in which the splash will be displayed
	virtual CRect& getDisplayArea (CRect& rect) const { rect = toDisplay; return rect; }	///< get the area in which the splash will be displayed

	virtual CMouseEventResult onMouseDown (CPoint& where, const long& buttons);

	CLASS_METHODS(CSplashScreen, CControl)

protected:
	void valueChanged (CControl *pControl);

	CRect	toDisplay;
	CRect	keepSize;
	CPoint	offset;
	CView* modalView;
};


//-----------------------------------------------------------------------------
// CVuMeter Declaration
//!
/// \nosubgrouping
//-----------------------------------------------------------------------------
class CVuMeter : public CControl
{
public:
	//-----------------------------------------------------------------------------
	/// \name Constructor
	//-----------------------------------------------------------------------------
	//@{
	CVuMeter (const CRect& size, CBitmap* onBitmap, CBitmap* offBitmap, long nbLed, const long style = kVertical);
	CVuMeter (const CVuMeter& vuMeter);
	//@}

	virtual ~CVuMeter ();	
  
	virtual void setDecreaseStepValue (float value) { decreaseValue = value; }

	virtual bool attached (CView* parent);
	virtual bool removed (CView* parent);
	virtual void draw (CDrawContext* pContext);
	virtual void setDirty (const bool val = true);
	
	void setUseOffscreen (bool val = true);
	bool getUseOffscreen () const { return bUseOffscreen; }

	CLASS_METHODS(CVuMeter, CControl)

protected:
	CBitmap* onBitmap;
	CBitmap* offBitmap;
	COffscreenContext* pOScreen;
	
	long     nbLed;
	long     style;
	float    decreaseValue;
	bool	 bUseOffscreen;

	CRect    rectOn;
	CRect    rectOff;
};

END_NAMESPACE_VSTGUI

#endif	// __vstcontrol__
