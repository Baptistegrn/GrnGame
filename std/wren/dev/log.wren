
class Log {
    // Logging
    foreign static log_error(message)
    foreign static log_info(message)
    foreign static log_warning(message)
    foreign static log_debug(message)
    foreign static log_critical(message)
    foreign static log_set_lvl(level)
    // Configuration
    foreign static apply_config(enable,destination)
    foreign static get_level()

}