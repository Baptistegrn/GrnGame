
class Log {
    foreign static error(message)
    foreign static info(message)
    foreign static warning(message)
    foreign static debug(message)
    foreign static critical(message)
    static warn(message){
        warning(message)
    }

}