import "std/wren/input/pad/pad_const" for PadConst

class PadAxis {
    static STICK_LX  { PadConst.new(PadConst.AXIS, 0) }
    static STICK_LY  { PadConst.new(PadConst.AXIS, 1) }
    static STICK_RX  { PadConst.new(PadConst.AXIS, 2) }
    static STICK_RY  { PadConst.new(PadConst.AXIS, 3) }
    static TRIGGER_L { PadConst.new(PadConst.AXIS, 4) }
    static TRIGGER_R { PadConst.new(PadConst.AXIS, 5) }

    static LEFT_X  { PadConst.new(PadConst.AXIS, 0) }
    static LEFT_Y  { PadConst.new(PadConst.AXIS, 1) }
    static RIGHT_X { PadConst.new(PadConst.AXIS, 2) }
    static RIGHT_Y { PadConst.new(PadConst.AXIS, 3) }

    static L2 { PadConst.new(PadConst.AXIS, 4) }
    static R2 { PadConst.new(PadConst.AXIS, 5) }

    static LEFT_TRIGGER  { PadConst.new(PadConst.AXIS, 4) }
    static RIGHT_TRIGGER { PadConst.new(PadConst.AXIS, 5) }
}