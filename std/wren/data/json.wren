class Json {
  foreign static create(key,value)
  foreign static open(key, min, max) // min, max interval for saving 
  foreign static contains(key)
  foreign static get(key)
  foreign static set(key, value)
  foreign static save(key)
  foreign static exist(key)
}