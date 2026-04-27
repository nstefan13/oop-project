#pragma once

#include <exception>
#include <stdexcept>
#include <string>

class ApplicationException : public std::exception {
  std::string message = "ApplicationException";

protected:
  explicit ApplicationException(const std::string &customMessage)
      : message{customMessage} {}

public:
  virtual const char *what() const throw() { return message.data(); };
};

class MissingNetworkParameterException : public ApplicationException {
public:
  explicit MissingNetworkParameterException(const std::string &message)
      : ApplicationException(message) {}
};

class InvalidStatusLineException : public ApplicationException {
public:
  explicit InvalidStatusLineException(const std::string &message)
      : ApplicationException(message) {}
};

class InvalidHeaderException : public ApplicationException {
public:
  explicit InvalidHeaderException(const std::string &message)
      : ApplicationException(message) {}
};