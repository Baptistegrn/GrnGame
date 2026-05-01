foreign class Db {
    construct new() {}
    foreign open(path)
    foreign prepare(sql)
    foreign close()
    foreign fetch(sql)
    foreign write(sql)
    foreign begin()
    foreign commit()
    foreign rollback()
    foreign static  exists(path)
}

//todo move in correct file 
foreign class DbStmt {
    construct new() {}
    foreign run(args)
    foreign free()
}