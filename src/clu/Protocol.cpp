#include "Protocol.h"
#include "../clu/Frame.h"
#include <arduino.h>

Protocol::Protocol()
{
}

Protocol::ProtocolStatus Protocol::Decode(const char *frameBuffer, Frame &frame)
{
  uint8_t frameLenght = strlen(frameBuffer);

  frame.Reset();
  if (frameLenght >= 6U && frameLenght % 2 == 0)
  {
    for (size_t i = 0; i < frameLenght; i++)
    {
      if (this->ConvertHexCharToInt(frameBuffer[i]) == 0xFF)
      {
        // char is invalid
        return Protocol::ProtocolStatus::ERROR_CHAR_INVALID;
      }
    }

    frame.clusterId = this->ConvertHexCharToInt(frameBuffer[0]) * 16 + this->ConvertHexCharToInt(frameBuffer[1]);
    frame.commandId = this->ConvertHexCharToInt(frameBuffer[2]) * 16 + this->ConvertHexCharToInt(frameBuffer[3]);
    frame.nbParams = this->ConvertHexCharToInt(frameBuffer[4]) * 16 + this->ConvertHexCharToInt(frameBuffer[5]);
    if (frame.nbParams == 0U && frameLenght == 6U)
    {
      return Protocol::ProtocolStatus::NO_ERROR;
    }
    else if ((frame.nbParams * 2U) + 6U == frameLenght)
    {
      for (size_t i = 0; i < frame.nbParams * 2U; i += 2U)
      {
        frame.params[i/2] = this->ConvertHexCharToInt(frameBuffer[6 + i]) * 16 + this->ConvertHexCharToInt(frameBuffer[7 + i]);
      }
      return Protocol::ProtocolStatus::NO_ERROR;
    }
    else
    {
      // wrong param size
      return Protocol::ProtocolStatus::ERROR_SIZE_PARAMS;
    }
  }
  else
  {
    // frameLenght is not or less than 6 bytes
    return Protocol::ProtocolStatus::ERROR_LENGHT;
  }
}

uint8_t Protocol::Encode(Frame &response, const uint8_t *buffer)
{
  if (buffer == nullptr || response.nbParams == 0 || response.params == nullptr)
  {
    return 0;
  }
  const uint8_t *params = response.params;
  const uint8_t size = response.nbParams;
  const uint8_t cluster = response.clusterId;
  const uint8_t command = response.commandId;

  uint8_t length = sprintf((char *)buffer, "<%02x%02x%02x", cluster, command, size);

  for (uint8_t i = 0; i < size; i++)
  {
    length += sprintf((char *)&buffer[length], "%02x", params[i]);
  }
  sprintf((char *)&buffer[length], ">");
  return strlen((char *)buffer);
}

uint8_t Protocol::ConvertHexCharToInt(uint8_t byte)
{
  if ((byte >= '0') && (byte <= '9'))
    return byte - '0';
  if ((byte >= 'A') && (byte <= 'F'))
    return byte + 10 - 'A';
  if ((byte >= 'a') && (byte <= 'f'))
    return byte + 10 - 'a';
  return 0xFF;
}