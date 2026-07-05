
class Log {
    foreign static error(message)
    foreign static info(message)
    foreign static warning(message)
    foreign static debug(message)
    foreign static critical(message)
    foreign static set_lvl(level)
    foreign static apply_config(enable,destination)
    foreign static get_level()

    static warn(message){
        warning(message)
    }

}