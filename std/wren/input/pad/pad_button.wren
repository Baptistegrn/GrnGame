import "std/wren/input/pad/pad_const" for PadConst

class PadButton {
    static PAD_SOUTH        { PadConst.new(PadConst.BUTTON, 0) }
    static PAD_EAST         { PadConst.new(PadConst.BUTTON, 1) }
    static PAD_WEST         { PadConst.new(PadConst.BUTTON, 2) }
    static PAD_NORTH        { PadConst.new(PadConst.BUTTON, 3) }
    static PAD_BACK         { PadConst.new(PadConst.BUTTON, 4) }
    static PAD_GUIDE        { PadConst.new(PadConst.BUTTON, 5) }
    static PAD_START        { PadConst.new(PadConst.BUTTON, 6) }
    static PAD_LS           { PadConst.new(PadConst.BUTTON, 7) }
    static PAD_RS           { PadConst.new(PadConst.BUTTON, 8) }
    static PAD_LB           { PadConst.new(PadConst.BUTTON, 9) }
    static PAD_RB           { PadConst.new(PadConst.BUTTON, 10) }
    static PAD_DPAD_UP      { PadConst.new(PadConst.BUTTON, 11) }
    static PAD_DPAD_DOWN    { PadConst.new(PadConst.BUTTON, 12) }
    static PAD_DPAD_LEFT    { PadConst.new(PadConst.BUTTON, 13) }
    static PAD_DPAD_RIGHT   { PadConst.new(PadConst.BUTTON, 14) }
    static PAD_MISC1        { PadConst.new(PadConst.BUTTON, 15) }
    static PAD_PADDLE1      { PadConst.new(PadConst.BUTTON, 16) }
    static PAD_PADDLE2      { PadConst.new(PadConst.BUTTON, 17) }
    static PAD_PADDLE3      { PadConst.new(PadConst.BUTTON, 18) }
    static PAD_PADDLE4      { PadConst.new(PadConst.BUTTON, 19) }
    static PAD_TOUCHPAD     { PadConst.new(PadConst.BUTTON, 20) }
    static PAD_MISC2        { PadConst.new(PadConst.BUTTON, 21) }
    static PAD_MISC3        { PadConst.new(PadConst.BUTTON, 22) }
    static PAD_MISC4        { PadConst.new(PadConst.BUTTON, 23) }
    static PAD_MISC5        { PadConst.new(PadConst.BUTTON, 24) }
    static PAD_MISC6        { PadConst.new(PadConst.BUTTON, 25) }

    static A { PadConst.new(PadConst.BUTTON, 0) }
    static B { PadConst.new(PadConst.BUTTON, 1) }
    static X { PadConst.new(PadConst.BUTTON, 2) }
    static Y { PadConst.new(PadConst.BUTTON, 3) }

    static LEFT_BUMPER      { PadConst.new(PadConst.BUTTON, 9) }
    static RIGHT_BUMPER     { PadConst.new(PadConst.BUTTON, 10) }
    static LEFT_STICK_CLICK { PadConst.new(PadConst.BUTTON, 7) }
    static RIGHT_STICK_CLICK{ PadConst.new(PadConst.BUTTON, 8) }
    static VIEW             { PadConst.new(PadConst.BUTTON, 4) }
    static MENU             { PadConst.new(PadConst.BUTTON, 6) }

    static UP    { PadConst.new(PadConst.BUTTON, 11) }
    static DOWN  { PadConst.new(PadConst.BUTTON, 12) }
    static LEFT  { PadConst.new(PadConst.BUTTON, 13) }
    static RIGHT { PadConst.new(PadConst.BUTTON, 14) }

    static CROSS    { PadConst.new(PadConst.BUTTON, 0) }
    static CIRCLE   { PadConst.new(PadConst.BUTTON, 1) }
    static SQUARE   { PadConst.new(PadConst.BUTTON, 2) }
    static TRIANGLE { PadConst.new(PadConst.BUTTON, 3) }

    static L1 { PadConst.new(PadConst.BUTTON, 9) }
    static R1 { PadConst.new(PadConst.BUTTON, 10) }
    static L3 { PadConst.new(PadConst.BUTTON, 7) }
    static R3 { PadConst.new(PadConst.BUTTON, 8) }

    static SHARE     { PadConst.new(PadConst.BUTTON, 4) }
    static OPTIONS   { PadConst.new(PadConst.BUTTON, 6) }
    static PS_BUTTON { PadConst.new(PadConst.BUTTON, 5) }
}