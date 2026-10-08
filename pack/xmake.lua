
includes("@builtin/xpack")

local stage   = os.getenv("GRNGAME_STAGE") or ""
local version = os.getenv("GRNGAME_VERSION") or "0.0.0"

xpack("grngame")
    set_title("GrnGame")
    set_author("Baptiste GUERIN  <baptiste.guerin34@gmail.com>")
    set_description("GrnGame platformer 2d")
    set_version(version)
    set_basename("grngame-$(plat)-$(arch)-$v(version)") 

    if is_plat("windows") then
        set_formats("wix")
        add_installfiles(stage .. "/(**)")
        before_installcmd(function (package, batchcmds)
            batchcmds:rawcmd("wix", [[
<Environment Id="GrnGamePath" Name="PATH" Value="[INSTALLFOLDER]scripts"
             Part="last" Action="set" System="yes" Permanent="no" />
]])
        end)
    elseif is_plat("linux") then
        set_formats("deb")
        add_installfiles(stage .. "/(**)", {prefixdir = "lib/grngame"})
        add_installfiles("launchers/(*)",  {prefixdir = "bin"})
    end