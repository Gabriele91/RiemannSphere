//
//  CocoaInput.cpp
//  Easy3D
//
//  Created by Gabriele Di Bari on 14/06/13.
//
//

#define ___PUBLIC_FOR_COCOA_CLASS
#include <CocoaInput.h>
#import <AppKit/NSOpenGLView.h>
#import <AppKit/NSWindow.h>
#import <AppKit/NSScreen.h>
#import <AppKit/NSPasteboard.h>
#import <string>
using namespace Easy3D;

#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_6
@interface CocoaWindowListener : NSResponder <NSWindowDelegate> {
#else
@interface CocoaWindowListener : NSResponder {
#endif        
	CocoaInput* Easy3Dinput;
    NSWindow* Easy3Dwindow;
}
    
-(id) init;
    
-(void) listen:(CocoaInput *)datainput :(NSWindow*) datawindow;
-(void) close;
    
/* Window delegate functionality */
-(BOOL) windowShouldClose:(id) sender;
-(void) windowDidResize:(NSNotification *) aNotification;
/* Window event handling */
-(void) mouseDown:(NSEvent *) theEvent;
-(void) rightMouseDown:(NSEvent *) theEvent;
-(void) otherMouseDown:(NSEvent *) theEvent;
-(void) mouseUp:(NSEvent *) theEvent;
-(void) rightMouseUp:(NSEvent *) theEvent;
-(void) otherMouseUp:(NSEvent *) theEvent;
-(void) mouseEntered:(NSEvent *)theEvent;
-(void) mouseExited:(NSEvent *)theEvent;
-(void) mouseMoved:(NSEvent *) theEvent;
-(void) mouseDragged:(NSEvent *) theEvent;
-(void) rightMouseDragged:(NSEvent *) theEvent;
-(void) otherMouseDragged:(NSEvent *) theEvent;
-(void) scrollWheel:(NSEvent *) theEvent;
    
    
@end
    
#define _onMouseDown(key)\
    {\
    Easy3Dinput->emouse.__mouseDown(key);\
    Easy3Dinput->__callOnMousePress(Easy3Dinput->emouse.pos,key);\
    }
    
#define _onMouseRelease(key)\
    {\
    Easy3Dinput->emouse.__mouseUp(key);\
    Easy3Dinput->__callOnMouseRelease(Easy3Dinput->emouse.pos,key);\
    }
    
    
@implementation CocoaWindowListener

- (id) init{
    self=[super init];
    //to null
    Easy3Dinput = NULL;
    Easy3Dwindow= NULL;
    return self;
}
    
- (void)listen:(CocoaInput *)datainput :(NSWindow*) datawindow
{
    Easy3Dinput = datainput;
    Easy3Dwindow=datawindow;
    NSNotificationCenter *center;
    NSView *view = [Easy3Dwindow contentView];
        
    center = [NSNotificationCenter defaultCenter];
        
    if ([Easy3Dwindow delegate] != nil) {
            [center addObserver:self selector:@selector(windowDidExpose:) name:NSWindowDidExposeNotification object:Easy3Dwindow];
            [center addObserver:self selector:@selector(windowDidMove:) name:NSWindowDidMoveNotification object:Easy3Dwindow];
            [center addObserver:self selector:@selector(windowDidResize:) name:NSWindowDidResizeNotification object:Easy3Dwindow];
            [center addObserver:self selector:@selector(windowDidMiniaturize:) name:NSWindowDidMiniaturizeNotification object:Easy3Dwindow];
            [center addObserver:self selector:@selector(windowDidDeminiaturize:) name:NSWindowDidDeminiaturizeNotification object:Easy3Dwindow];
            [center addObserver:self selector:@selector(windowDidBecomeKey:) name:NSWindowDidBecomeKeyNotification object:Easy3Dwindow];
            [center addObserver:self selector:@selector(windowDidResignKey:) name:NSWindowDidResignKeyNotification object:Easy3Dwindow];
    }
    else {
        [Easy3Dwindow setDelegate:self];
    }
        
    [Easy3Dwindow setNextResponder:self];
    [Easy3Dwindow setAcceptsMouseMovedEvents:YES];
        
    [view setNextResponder:self];
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
    [view setAcceptsTouchEvents:YES];
#endif
}
    
- (void)close
{
    if(Easy3Dwindow){
        NSNotificationCenter *center;
        NSView *view = [Easy3Dwindow contentView];
            
        center = [NSNotificationCenter defaultCenter];

        if ([Easy3Dwindow delegate] != self) {
            [center removeObserver:self name:NSWindowDidExposeNotification object:Easy3Dwindow];
            [center removeObserver:self name:NSWindowDidMoveNotification object:Easy3Dwindow];
            [center removeObserver:self name:NSWindowDidResizeNotification object:Easy3Dwindow];
            [center removeObserver:self name:NSWindowDidMiniaturizeNotification object:Easy3Dwindow];
            [center removeObserver:self name:NSWindowDidDeminiaturizeNotification object:Easy3Dwindow];
            [center removeObserver:self name:NSWindowDidBecomeKeyNotification object:Easy3Dwindow];
            [center removeObserver:self name:NSWindowDidResignKeyNotification object:Easy3Dwindow];
        }
        else {
            [Easy3Dwindow setDelegate:nil];
        }
        if ([Easy3Dwindow nextResponder] == self) {
            [Easy3Dwindow setNextResponder:nil];
        }
        if ([view nextResponder] == self) {
            [view setNextResponder:nil];
        }
    }
    //to null
    Easy3Dinput = NULL;
    Easy3Dwindow= NULL;
}

/* Window delegate functionality */
-(BOOL) windowShouldClose:(id) sender {
    Easy3Dinput->ewindow.close=true;//send close message
    return false; //stop close window (this window will close after exit main loop)
}
-(void) windowDidResize:(NSNotification *) aNotification{
    //todo
}
/* Window event handling */
-(void) mouseDown:(NSEvent *) theEvent{
    _onMouseDown(Key::BUTTON_LEFT);
}
-(void) rightMouseDown:(NSEvent *) theEvent{    
    _onMouseDown(Key::BUTTON_RIGHT);
}
-(void) otherMouseDown:(NSEvent *) theEvent{
    _onMouseDown(Key::BUTTON_MIDDLE);
}
    
-(void) mouseUp:(NSEvent *) theEvent{
    _onMouseRelease(Key::BUTTON_LEFT);
}
-(void) rightMouseUp:(NSEvent *) theEvent{
    _onMouseRelease(Key::BUTTON_RIGHT);

}
-(void) otherMouseUp:(NSEvent *) theEvent{
    _onMouseRelease(Key::BUTTON_MIDDLE);

}
-(void) mouseEntered:(NSEvent *)theEvent{
    //focus == true
    Easy3Dinput->ewindow.focus=true;    
}
-(void) mouseExited:(NSEvent *)theEvent{
    //clear event, focus ==false
    Easy3Dinput->ewindow.focus=false;
	//input keyboard hit
	Easy3Dinput->ekeyboard.__init();
	//input mouse hit
	Easy3Dinput->emouse.__init();
}
-(void) mouseMoved:(NSEvent *) theEvent{
    NSPoint point=[theEvent locationInWindow];
    Easy3Dinput->emouse.pos.x=(int)point.x;
    Easy3Dinput->emouse.pos.y=(Application::instance()->getScreen()->getHeight()-(int)(point.y));
    Easy3Dinput->__callOnMouseMove(Easy3Dinput->emouse.pos);
}
-(void) mouseDragged:(NSEvent *) theEvent{    
	[self mouseMoved:theEvent];
}
-(void) rightMouseDragged:(NSEvent *) theEvent{
    [self mouseMoved:theEvent];
}
-(void) otherMouseDragged:(NSEvent *) theEvent{
    [self mouseMoved:theEvent];
}
-(void) scrollWheel:(NSEvent *) theEvent{
    Easy3Dinput->emouse.scroll=[theEvent deltaY];
    Easy3Dinput->__callOnMouseScroll(Easy3Dinput->emouse.scroll);
}
    
#undef _onMouseDown
#undef _onMouseRelease
@end
    
//Key::KEYBOARDMAX
Key::Keyboard KeyMapCocoa[]={
    /*   0 */   Key::A,
    /*   1 */   Key::S,
    /*   2 */   Key::D,
    /*   3 */   Key::F,
    /*   4 */   Key::H,
    /*   5 */   Key::G,
    /*   6 */   Key::Z,
    /*   7 */   Key::X,
    /*   8 */   Key::C,
    /*   9 */   Key::V,
    /*  10 */   Key::BACKSLASH,//Key::NONUSBACKSLASH,
    /* 
        Key::NONUSBACKSLASH on ANSI and JIS keyboards (if this key would exist there), Key::GRAVE on ISO.
       (The USB keyboard driver actually translates these usage codes to different virtual key codes depending on whether the keyboard is ISO/ANSI/JIS.
        That's why you have to help it identify the keyboard type when you plug in a PC USB keyboard. It's a historical thing - ADB keyboards are wired this way.) 
     */
    /*  11 */   Key::B,
    /*  12 */   Key::Q,
    /*  13 */   Key::W,
    /*  14 */   Key::E,
    /*  15 */   Key::R,
    /*  16 */   Key::Y,
    /*  17 */   Key::T,
    /*  18 */   Key::N1,
    /*  19 */   Key::N2,
    /*  20 */   Key::N3,
    /*  21 */   Key::N4,
    /*  22 */   Key::N6,
    /*  23 */   Key::N5,
    /*  24 */   Key::EQUALS,
    /*  25 */   Key::N9,
    /*  26 */   Key::N7,
    /*  27 */   Key::MINUS,
    /*  28 */   Key::N8,
    /*  29 */   Key::N0,
    /*  30 */   Key::RIGHTBRACKET,
    /*  31 */   Key::O,
    /*  32 */   Key::U,
    /*  33 */   Key::LEFTBRACKET,
    /*  34 */   Key::I,
    /*  35 */   Key::P,
    /*  36 */   Key::RETURN,
    /*  37 */   Key::L,
    /*  38 */   Key::J,
    /*  39 */   Key::APOSTROPHE,
    /*  40 */   Key::K,
    /*  41 */   Key::SEMICOLON,
    /*  42 */   Key::BACKSLASH,
    /*  43 */   Key::COMMA,
    /*  44 */   Key::SLASH,
    /*  45 */   Key::N,
    /*  46 */   Key::M,
    /*  47 */   Key::PERIOD,
    /*  48 */   Key::TAB,
    /*  49 */   Key::SPACE,
    /*  50 */   Key::KEY_NULL,//Key::GRAVE, /* Key::GRAVE on ANSI and JIS keyboards, Key::NONUSBACKSLASH on ISO (see comment about virtual key code 10 above) */
    /*  51 */   Key::BACKSPACE,
    /*  52 */   Key::KEY_NULL,//Key::KP_ENTER, /* keyboard enter on portables */
    /*  53 */   Key::ESCAPE,
    /*  54 */   Key::LMETA,//Key::RGUI,
    /*  55 */   Key::RMETA,//Key::LGUI,
    /*  56 */   Key::LSHIFT,
    /*  57 */   Key::CAPSLOCK,
    /*  58 */   Key::LALT,
    /*  59 */   Key::LCTRL,
    /*  60 */   Key::RSHIFT,
    /*  61 */   Key::RALT,
    /*  62 */   Key::RCTRL,
    /*  63 */   Key::RMETA,//Key::RGUI, /* fn on portables, acts as a hardware-level modifier already, so we don't generate events for it, also XK_Meta_R */
    /*  64 */   Key::KEY_NULL, /* unknown (unused?) */
    /*  65 */   Key::KP_PERIOD,
    /*  66 */   Key::KEY_NULL, /* unknown (unused?) */
    /*  67 */   Key::KP_MULTIPLY,
    /*  68 */   Key::KEY_NULL, /* unknown (unused?) */
    /*  69 */   Key::KP_PLUS,
    /*  70 */   Key::KEY_NULL, /* unknown (unused?) */
    /*  71 */   Key::KEY_NULL,//Key::NUMLOCKCLEAR,
    /*  72 */   Key::KEY_NULL,//Key::VOLUMEUP,
    /*  73 */   Key::KEY_NULL,//Key::VOLUMEDOWN,
    /*  74 */   Key::KEY_NULL,//Key::MUYR,
    /*  75 */   Key::KP_DIVIDE,
    /*  76 */   Key::KEY_NULL,//Key::KP_ENTER, /* keypad enter on external keyboards, fn-return on portables */
    /*  77 */   Key::KEY_NULL, /* unknown (unused?) */
    /*  78 */   Key::KP_MINUS,
    /*  79 */   Key::KEY_NULL, /* unknown (unused?) */
    /*  80 */   Key::KEY_NULL, /* unknown (unused?) */
    /*  81 */   Key::KEY_NULL,//Key::KP_EQUALS,
    /*  82 */   Key::KP0,
    /*  83 */   Key::KP1,
    /*  84 */   Key::KP2,
    /*  85 */   Key::KP3,
    /*  86 */   Key::KP4,
    /*  87 */   Key::KP5,
    /*  88 */   Key::KP6,
    /*  89 */   Key::KP7,
    /*  90 */   Key::KEY_NULL, /* unknown (unused?) */
    /*  91 */   Key::KP8,
    /*  92 */   Key::KP9,
    /*  93 */   Key::KEY_NULL,//Key::INTERNATIONAL3, /* Cosmo_USB2ADB.c says "Yen (JIS)" */
    /*  94 */   Key::KEY_NULL,//Key::INTERNATIONAL1, /* Cosmo_USB2ADB.c says "Ro (JIS)" */
    /*  95 */   Key::KEY_NULL,//Key::KP_COMMA, /* Cosmo_USB2ADB.c says ", JIS only" */
    /*  96 */   Key::F5,
    /*  97 */   Key::F6,
    /*  98 */   Key::F7,
    /*  99 */   Key::F3,
    /* 100 */   Key::F8,
    /* 101 */   Key::F9,
    /* 102 */   Key::KEY_NULL,//Key::LANG2, /* Cosmo_USB2ADB.c says "Eisu" */
    /* 103 */   Key::F11,
    /* 104 */   Key::KEY_NULL,//Key::LANG1, /* Cosmo_USB2ADB.c says "Kana" */
    /* 105 */   Key::PRINT,//Key::PRINTSCREEN,
    /* On ADB keyboards, this key is labeled "F13/print screen". Problem: USB has different usage codes for these two functions. On Apple USB keyboards, the key is labeled "F13" and sends the F13 usage code (Key::F13). I decided to use Key::PRINTSCREEN here nevertheless since SDL applications are more likely to assume the presence of a print screen key than an F13 key. */
    /* 106 */   Key::KEY_NULL,//Key::F16,
    /* 107 */   Key::SCROLLOCK,//Key::SCROLLLOCK, /* F14/scroll lock, see comment about F13/print screen above */
    /* 108 */   Key::KEY_NULL, /* unknown (unused?) */
    /* 109 */   Key::F10,
    /* 110 */   Key::KEY_NULL,//Key::APPLICATION, /* windows contextual menu key, fn-enter on portables */
    /* 111 */   Key::F12,
    /* 112 */   Key::KEY_NULL, /* unknown (unused?) */
    /* 113 */   Key::PAUSE, /* F15/pause, see comment about F13/print screen above */
    /* 114 */   Key::INSERT,
    /* the key is actually labeled "help" on Apple keyboards, and works as such in Mac OS, but it sends the "insert" usage code even on Apple USB keyboards */
    /* 115 */   Key::HOME,
    /* 116 */   Key::PAGEUP,
    /* 117 */   Key::KEY_NULL,//Key::DELETE,
    /* 118 */   Key::F4,
    /* 119 */   Key::END,
    /* 120 */   Key::F2,
    /* 121 */   Key::PAGEDOWN,
    /* 122 */   Key::F1,
    /* 123 */   Key::LEFT,
    /* 124 */   Key::RIGHT,
    /* 125 */   Key::DOWN,
    /* 126 */   Key::UP,
    /* 127 */   Key::POWER
};

    
CocoaInput::CocoaInput(){
	//make listener
    listener=[[CocoaWindowListener alloc] init];
	//input window
	ewindow.__init();
	//input keyboard hit
	ekeyboard.__init();
	//input mouse hit
	emouse.__init();
    
}

CocoaInput::~CocoaInput(){
    CocoaWindowListener* listener=(CocoaWindowListener*)this->listener;
    [listener close];
    [listener release];    
}
    
    
/**
 * copy a string
 */
void CocoaInput::copyString(const String& paste){
    NSPasteboard *pasteBoard = [NSPasteboard generalPasteboard];
    [pasteBoard declareTypes:[NSArray arrayWithObjects:NSStringPboardType, nil] owner:nil];
    [pasteBoard setString: @(paste.c_str()) forType:NSStringPboardType];
}
/**
 * paste a string
 */
String CocoaInput::pasteString(){
    NSPasteboard*  myPasteboard  = [NSPasteboard generalPasteboard];
    NSString* myString = [myPasteboard  stringForType:NSPasteboardTypeString];
    return [myString UTF8String];
}
void CocoaInput::__updateCocoaEvent(){
    
#define keyDownEvent(key) \
{ \
if(ekeyboard.status[KeyMapCocoa[(key)]]==0){\
ekeyboard.__keyboardDown(KeyMapCocoa[(key)]);\
__callOnKeyPress(KeyMapCocoa[(key)]);\
}\
}
    
#define keyReleaseEvent(key) \
{ \
ekeyboard.__keyboardUp(KeyMapCocoa[(key)]);\
__callOnKeyRelease(KeyMapCocoa[(key)]);\
}
    //get events
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    //flag key status
    static unsigned int oldFlags = 0;
    
	for ( ; ; ) {
		NSEvent *event = [NSApp nextEventMatchingMask:NSAnyEventMask
                                untilDate:[NSDate distantPast]
                                inMode:NSDefaultRunLoopMode dequeue:YES ];
		if ( event == nil ) {
			break;
		}
        //flags
        unsigned int flags = (unsigned int)[event modifierFlags];
        //get char input
		switch ([event type]) {
			case NSKeyDown:
                inputString = [[event charactersIgnoringModifiers] UTF8String];
                keyDownEvent(event.keyCode);
            break;
			case NSKeyUp:
                keyReleaseEvent(event.keyCode);                
                /* Add to support system-wide keyboard shortcuts like CMD+Space */
				if (([event modifierFlags] & NSCommandKeyMask) || [event type] == NSFlagsChanged)
					[NSApp sendEvent: event];
            break;
            case NSFlagsChanged:
                //reset
                inputString="";
                //special chars
                if ((flags & NSControlKeyMask) != (oldFlags & NSControlKeyMask)){
                    if(flags & NSControlKeyMask){
                        ekeyboard.__keyboardDown(Key::RCTRL);
                        __callOnKeyPress(Key::RCTRL);
                    }else{
                        ekeyboard.__keyboardUp(Key::RCTRL);
                        __callOnKeyRelease(Key::RCTRL);
                    }
                }
                if ((flags & NSCommandKeyMask) != (oldFlags & NSCommandKeyMask)){
                    if(flags & NSCommandKeyMask){
                        ekeyboard.__keyboardDown(Key::RCTRL);
                        __callOnKeyPress(Key::RCTRL);
                    }else{
                        ekeyboard.__keyboardUp(Key::RCTRL);
                        __callOnKeyRelease(Key::RCTRL);
                    }
                }
                if ((flags & NSAlternateKeyMask) != (oldFlags & NSAlternateKeyMask)){
                    if(flags & NSAlternateKeyMask){
                        ekeyboard.__keyboardDown(Key::RALT);
                        __callOnKeyPress(Key::RALT);
                    }else{
                        ekeyboard.__keyboardUp(Key::RALT);
                        __callOnKeyRelease(Key::RALT);
                    }
                }
                if ((flags & NSShiftKeyMask) != (oldFlags & NSShiftKeyMask)){
                    if(flags & NSShiftKeyMask){
                        ekeyboard.__keyboardDown(Key::RSHIFT);
                        __callOnKeyPress(Key::RSHIFT);
                    }else{
                        ekeyboard.__keyboardUp(Key::RSHIFT);
                        __callOnKeyRelease(Key::RSHIFT);
                    }
                }
                oldFlags = flags;
            break;
			default:
				[NSApp sendEvent:event];
            break;
		}
	}
    
	[pool release];
    
#undef keyDownEvent
#undef keyReleaseEvent
}

void CocoaInput::update(){
	//update input
	//reset input window
	ewindow.resize=false;
	//update hit
	ekeyboard.__clearHit();
	emouse.__clearHit();
	//update cocoa events
    __updateCocoaEvent();
	///////////////////////////LOOP EVENT
	//update down keys
	ekeyboard.__update(this);
	emouse.__update(this);
    
}
//cocoa listener
void CocoaInput::__addCocoaListener(void *nswindow){
    CocoaWindowListener* listener=(CocoaWindowListener*)this->listener;
    [listener listen:(Easy3D::CocoaInput *)this :(NSWindow *)((NSWindow *)nswindow)];
}
void CocoaInput::__closeCocoaListener(){
    CocoaWindowListener* listener=(CocoaWindowListener*)this->listener;
    [listener close];
}
//calls
void CocoaInput::__callOnKeyPress(Key::Keyboard key) {
    for(size_t i=0;i!=vkeyboardh.size();++i)
        vkeyboardh[i]->onKeyPress(key);
}
void CocoaInput::__callOnKeyRelease(Key::Keyboard key) {
    for(size_t i=0;i!=vkeyboardh.size();++i)
        vkeyboardh[i]->onKeyRelease(key);
}
void CocoaInput::__callOnKeyDown(Key::Keyboard key) {
    for(size_t i=0;i!=vkeyboardh.size();++i)
        vkeyboardh[i]->onKeyDown(key);
}
//mouse
void CocoaInput::__callOnMouseMove(Vec2 mousePosition) {
    for(size_t i=0;i!=vmouseh.size();++i)
        vmouseh[i]->onMouseMove(mousePosition);
}
void CocoaInput::__callOnMousePress(Vec2 mousePosition, Key::Mouse button) {
    for(size_t i=0;i!=vmouseh.size();++i)
        vmouseh[i]->onMousePress(mousePosition,button);
}
void CocoaInput::__callOnMouseDown(Vec2 mousePosition, Key::Mouse button) {
    for(size_t i=0;i!=vmouseh.size();++i)
        vmouseh[i]->onMouseDown(mousePosition,button);
}
void CocoaInput::__callOnMouseRelease(Vec2 mousePosition, Key::Mouse button) {
    for(size_t i=0;i!=vmouseh.size();++i)
        vmouseh[i]->onMouseRelease(mousePosition,button);
}
void CocoaInput::__callOnMouseScroll(short scrollDelta) {
    for(size_t i=0;i!=vmouseh.size();++i)
        vmouseh[i]->onMouseScroll(scrollDelta);
}