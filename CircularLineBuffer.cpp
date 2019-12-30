#include "CircularLineBuffer.h"
#include <iostream>
using namespace std;

bool CircularLineBuffer::_writeChars(const char *chars, size_t nchars) {
  if (freeSpace() >= nchars) {
    int n = 0;
    int startingIndex = nextFreeIndex();
    if (startingIndex + nchars <= bufferSize) {
      for (int i = startingIndex; i < startingIndex + nchars; i++) {
        buffer[i] = chars[n];
        n++;
      }
    } else {
      for (int i = startingIndex; i < bufferSize; i++) {
        buffer[i] = chars[n];
        n++;
      }
      for (int i = 0; i < nchars - (bufferSize - startingIndex); i++) {
        buffer[i] = chars[n];
        n++;
      }
    }
    count += nchars;
    return true;
  } else {
    return false;
  }
}

string CircularLineBuffer::_readLine() {
  string bufferString = "";
  if (hasLine()) {
    int i = start;
    int n = 0;
    while (i != findNewline()) {
      bufferString += buffer[i];
      i = (i + 1) % bufferSize;
      n++;
    }
    start += (n + 1) % bufferSize;
    count -= (n + 1);
    bufferString += "\n";
    return bufferString;
  } else {
    return {}; //empty string (c++11)
  }
}

int CircularLineBuffer::freeSpace() {
  int freespace = bufferSize - count;
  return freespace;
}

bool CircularLineBuffer::isFull() {
  if (freeSpace() == 0) {
    return true;
  }
  return false;
}

bool CircularLineBuffer::isEmpty() {
  if (count == 0) {
    return true;
  }
  return false;
}

int CircularLineBuffer::nextFreeIndex() {
  int res;
  if (!isFull()) {
    if (start == 0 && count == 0) {
      int res = start;
      return res;
    }
    else {
      int res = (start + count) % bufferSize;
      return res;
    }
  }
}

int CircularLineBuffer::findNewline() {
  if (hasLine()) {
    for (int i = start; i < bufferSize; i++) {
      if (buffer[i] == '\n') {
        return i;
      }
    }
  }
}

bool CircularLineBuffer::hasLine() { //mind the deleted ones
  int endPos;
  endPos = start + count;
  if (endPos <= bufferSize) {
    for (int i = start; i < endPos; i++) {
      if (buffer[i] == '\n') {
        return true;
      }
    }
  } else {
    endPos = (start + count) % bufferSize;
    for (int i = start; i < bufferSize; i++) {
      if (buffer[i] == '\n') {
        return true;
      }
    }
    for (int i = 0; i < endPos; i++) {
      if (buffer[i] == '\n') {
        return true;
      }
    }
  }
  return false;
}
