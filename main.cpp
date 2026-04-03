#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <inja/inja.hpp>
#include "include/Utilities.h"
#include "include/HTTPRequest.h"
#include "include/CurlSession.h"
#include "include/TemplatedRequest.h"

int main(void)
{
  std::cout << "\nProcessing request from std::cin\n";
  std::string sablon = readEverything(std::cin);

  TemplatedRequest treq(sablon);
  HTTPRequest request = treq.compileRequest("PULA_MEA_IN_PIZDA_MATII");
  HTTPResponse response = request.perform().value();
  std::cout << response.get_body().substr(0, 1000);

  // Se citeste de la tastatura un HTTP request-uri valid, care apoi se executa
  // De ce ai face asta? Pentru ca inainte de a executa request-ul sa il pot modifica si sa pot injecta
  // payload-uri care ar putea duce la un data leak. Cum oricum trebuie sa construiesc un request,
  // mai degraba las utilizatorul sa-mi dea unul care merge decat sa fac-l fac de la 0.

  // try {
  //   std::cout << "Simple request to example.com:\n";
  //   auto session = CurlSession();
  //   auto response = session.request("GET", "https://example.com", HTTPHeaders(), "").value();
  //   std::cout << response << '\n';
  //   std::cout << response.get_body() << '\n';
  
  //   std::cout << "\nProcessing request from std::cin\n";
  //   auto request = HTTPRequest(std::cin);
  //   auto result = request.perform();
  //   if (result) {
  //     std::cout << result.value() << "\n";
  //     std::cout << result.value().get_body() << "\n";
  //   } else {
  //     std::cout << "Error while sending the request: " << result << "\n";
  //   }
  // } catch(...) {
  //   std::cerr << "Something went wrong";
  // }
}
