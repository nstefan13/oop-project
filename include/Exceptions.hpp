#pragma once

#include <exception>
#include <stdexcept>
#include <string>

class ApplicationException : public std::exception {
  std::string message = "ApplicationException";

protected:
  ApplicationException(const std::string &customMessage)
      : message{customMessage} {}

public:
  virtual const char *what() const throw() { return message.data(); };
};

class MissingNetworkParameterException : public ApplicationException {
public:
  MissingNetworkParameterException(const std::string &message)
      : ApplicationException(message) {}
};

class InvalidStatusLineException : public ApplicationException {
public:
  InvalidStatusLineException(const std::string &message)
      : ApplicationException(message) {}
};

class InvalidHeaderException : public ApplicationException {
public:
  InvalidHeaderException(const std::string &message)
      : ApplicationException(message) {}
};