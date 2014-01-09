import time; import serial; import regs_pb2

def pb_invoke(request, response):
  ser = serial.Serial('/dev/ttyACM3', 921600, timeout=1)
  request.timestamp = int(time.time())
  try:
    start = time.time()
    ser.write(request.SerializeToString()+'\x00')
    response.ParseFromString(ser.readall())
    duration = time.time() - start
  finally:
    ser.close()
  print response
  print 'duration: ',duration
  print 'rate: ', request.count/duration
  
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

def get_samples(count):
  request = regs_pb2.Request()
  response = regs_pb2.Response()
  request.action=regs_pb2.Request.SAMPLES
  request.count = count
  pb_invoke(request, response)
