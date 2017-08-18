/*
  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
  版权所有: @老潘orz  wasdpkj@hotmail.com

  本例程展示了一种占用内存资源极低的字符串处理办法
*/

#define BUFFER_CLR 0
#define BUFFER_SET 1

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

bool available(String begin = "+IPD", String end = ":", uint32_t timeout = 10000) {
  bool sEnd = bool(end.length() != 0);
  uint8_t max_length = begin.length();
  char * cache_data = "0123456789ABCDEF";
  memset(cache_data, 0, 16);

  int16_t n = 0;
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
        if (String(cache_data).indexOf(end) != -1) {
          return true;
        }
      }
      else if (String(cache_data).indexOf(begin) != -1) {
        if (!sEnd) {  //没end
          return true;
        }

        max_length = end.length();  //结束标志长度
        memset(cache_data, 0, 16); //缓存归零
        n = 0;            //重新开始写入缓存
        s = true;
      }

    }
  }

  return false;
}


bool available(String begin = "+IPD", uint32_t timeout = 10000) {
  return available(begin, "", timeout);
}

bool available(uint32_t timeout) {
  return available("+IPD", "", timeout);
}


/*
    Recvive data from uart. Return from begin to end received data if target found or timeout.
*/
bool recvStringAt(String begin, String end, String &data, uint32_t timeout = 10000, bool mode = true) {
  bool sEnd = bool(end.length() != 0);
  uint8_t max_length = begin.length();
  char * cache_data = "0123456789ABCDEF";
  memset(cache_data, 0, 16);

  int16_t n = 0;
  char a;
  bool s = false;
  unsigned long start = millis();
  while (millis() - start < timeout) {
    while (d_puart->available() > 0) {
      a = d_puart->read();
      if (a == '\0') continue;
      //Serial.write(a);

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

        if (sEnd) { //有end
          if (String(cache_data).indexOf(end) != -1) {
            if (mode == BUFFER_CLR) rx_empty();
            return true;
          }
        }
      }
      else if (String(cache_data).indexOf(begin) != -1) {
        data += cache_data;      //开始标志赋值
        s = true;

        if (sEnd) { //有end
          max_length = end.length();  //结束标志长度
          memset(cache_data, 0, 16); //缓存归零
          n = 0;            //重新开始写入缓存
        }
      }

    }
  }

  if (sEnd) {  //有end
    data = "NULL";
    return false;
  }
  else {  //无end
    if (!s) data = "NULL";
    return s;
  }
}


/*
   Recvive data from uart. Return from begin received data if target found or timeout.
*/
bool recvStringAt(String begin, String &data, uint32_t timeout = 10000) {
  return recvStringAt(begin, "", data, timeout);
}


String findAndFilter(String target, String begin, String end, String data, uint8_t bufferNum = 32) {
  if (data.indexOf(target) != -1) {
    int16_t index1 = data.indexOf(begin);
    if (index1 == -1) {
      return "NULL";
    }
    index1 += begin.length();
    data = data.substring(index1, index1 + bufferNum);

    int16_t index2 = data.indexOf(end);
    if (index2 != -1) {
      return data.substring(0, index2);
    }
  }
  return "NULL";
}