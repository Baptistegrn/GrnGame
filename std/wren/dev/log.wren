import "config" for Config

class Log {
    // Logging
    foreign static log_error(message)
    foreign static log_info(message)
    foreign static log_warning(message)
    foreign static log_debug(message)
    foreign static log_critical(message)
    foreign static log_set_lvl(level)

    // Configuration
    foreign static apply_config()

    foreign static set_enable(enable)
    foreign static set_destination(destination)

    foreign static get_enable()

    static apply(){
        set_enable(Config.enableLogs)
        set_destination(Config.logDestination)
        apply_config()
    }
}