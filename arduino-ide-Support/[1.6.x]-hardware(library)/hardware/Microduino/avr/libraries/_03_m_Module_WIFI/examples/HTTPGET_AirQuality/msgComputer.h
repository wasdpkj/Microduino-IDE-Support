/*
  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
  版权所有: @老潘orz  wasdpkj@hotmail.com

  本例程展示了一种占用内存资源极低的字符串处理办法
*/

Stream *d_puart;

void msgInit(Stream *d) {
  d_puart = d;
}

void freeRam() {
  extern int __heap_start, *__brkval;
  int v;
  Serial.print(F("freeRAM:"));
  Serial.print( (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval));
  Serial.print(F("\r\n"));
}

void rx_empty() {
  while (d_puart->available() > 0) {
    d_puart->read();
  }
}

/*
   Recvive data from uart. Return from begin received data if target found or timeout.
*/
bool recvStringAt(String begin, String &data, uint32_t timeout = 10000) {
  uint8_t begin_length = begin.length();
  char * cache_data = "0123456789ABCDEF";
  memset(cache_data, 0, 16);

  uint32_t n = 0;
  char a;
  bool s = false;
  unsigned long start = millis();
  while (millis() - start < timeout) {
    while (d_puart->available() > 0) {
      a = d_puart->read();
      if (a == '\0') continue;

      if (n > begin_length - 1) {
        memmove(cache_data, cache_data + 1, begin_length - 1);
        cache_data[begin_length - 1] = a;
      }
      else {
        cache_data[n] = a;
        n++;
      }

      if (s) {
        data += a;
      }
      else if (String(cache_data).indexOf(begin) != -1) {
        data += cache_data;
        s = true;
      }

    }
  }

  if (!s)   data = "NULL";
  return s;
}

/*
    Recvive data from uart. Return from begin to end received data if target found or timeout.
*/
bool recvStringAt(String begin, String end, String &data, uint32_t timeout = 10000, bool mode = true) {
  uint8_t max_length = begin.length();
  char * cache_data = "0123456789ABCDEF";
  memset(cache_data, 0, 16);

  uint32_t n = 0;
  char a;
  bool s = false;
  unsigned long start = millis();
  while (millis() - start < timeout) {
    while (d_puart->available() > 0) {
      a = d_puart->read();
      if (a == '\0') continue;

      if (n > max_length - 1) {
        memmove(cache_data, cache_data + 1, max_length - 1);
        cache_data[max_length - 1] = a;
      }
      else {
        cache_data[n] = a;
        n++;
      }

      if (s) {
        data += a;
        if (String(cache_data).indexOf(end) != -1) {
          if (mode) rx_empty();
          return true;
        }
      }
      else if (String(cache_data).indexOf(begin) != -1) {
        data += cache_data;      //开始标志赋值
        max_length = end.length();  //结束标志长度
        memset(cache_data, 0, 16); //缓存归零
        n = 0;            //重新开始写入缓存
        s = true;
      }

    }
  }

  data = "NULL";
  return false;
}

String recvFindAndFilter(String target, String begin, String end, String data, uint8_t bufferNum = 8) {
  if (data.indexOf(target) != -1) {
    int16_t index1 = data.indexOf(begin);
    index1 += begin.length();
    data = data.substring(index1, index1 + bufferNum);
    int32_t index2 = data.indexOf(end);
    index1 = 0;
    if (index1 != -1 && index2 != -1) {
      return data.substring(index1, index2);
    }
  }
  return "NULL";
}
