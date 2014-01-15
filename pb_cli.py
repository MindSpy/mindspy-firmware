import time; import serial; import regs_pb2

def pb_invoke(request, response):
  ser = serial.Serial('/dev/ttyACM3', 921600, timeout=1)
  request.timestamp = int(time.time())
  i = 0
  err = 0
  try:
    start = time.time()
    ser.flushInput()
    ser.write(request.SerializeToString()+'\x00')
    if request.repeat:
      buff = ''
      sep = regs_pb2.Separator()
      sep.last = False
      sep = sep.SerializeToString()
      while True:
        try:
          buff += ser.read(ser.inWaiting())
          if buff:
            buff = buff.split(sep)
            if len(buff)>1:
              response.Clear()
              for m in buff[:-1]:
                try:
                  response.MergeFromString(m)
                except:
                  err += 1
                i += len(response.sample)
              buff = buff[-1]
              duration = time.time() - start
              print response
              print 'duration: ', duration
              print 'rate: ', i/duration
            else:
              buff = buff[0]
        except KeyboardInterrupt:
          request.action=regs_pb2.Request.ECHO
          request.repeat = False
          ser.write(request.SerializeToString()+'\x00')
          buff += ser.readall()
          try:
            response.ParseFromString(buff)
            print response
          except:
            err += 1
          break 
    else:
      response.ParseFromString(ser.readall())
      print response
      i = len(response.sample)
    duration = time.time() - start
  finally:
    ser.close()
  print 'duration: ',duration
  print 'rate: ', i/duration
  print 'errors: ', err
  
def echo():
  request = regs_pb2.Request()
  response = regs_pb2.Response()
  request.action=regs_pb2.Request.ECHO
  pb_invoke(request, response)

def get_state(start, count):
  request = regs_pb2.Request()
  response = regs_pb2.Response()
  request.action=regs_pb2.Request.GET_STATE
  request.count=count
  request.start = start
  pb_invoke(request, response)

def set_state(start, payload):
  request = regs_pb2.Request()
  response = regs_pb2.Response()
  request.action=regs_pb2.Request.SET_STATE
  request.payload = payload
  request.start = start
  pb_invoke(request, response)

def get_samples(count, repeat = False):
  request = regs_pb2.Request()
  response = regs_pb2.Response()
  request.action=regs_pb2.Request.SAMPLES
  request.count = count
  request.repeat = repeat
  pb_invoke(request, response)
