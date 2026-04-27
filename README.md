# NoSQLMap

A tool similar to SQLMap ment to pentest endpoints that interact with a NoSQL database and exfiltrate data / achieve remote code execution.

# Detalli punctare

Aveți voie cu cod generat de modele de limbaj la care nu ați contribuit semnificativ doar dacă documentați riguros acest proces.
Codul generat pus "ca să fie"/pe care nu îl înțelegeți se punctează doar pentru puncte bonus, doar în contextul
în care oferă funcționalități ajutătoare și doar dacă are sens.

Codul din proiect trebuie să poată fi ușor de înțeles și de modificat de către altcineva. Pentru detalii, veniți la ore.

O cerință nu se consideră îndeplinită dacă este realizată doar prin cod generat.

- **Fără cod de umplutură/fără sens!**
- **Fără copy-paste!**
- **Fără variabile globale!**
- **Fără atribute publice!**
- **Pentru T2 și T3, fără date în cod!** Datele vor fi citite din fișier, aveți exemple destule.
- **Obligatoriu** fișiere cu date mai multe din care să citiți, obligatoriu cu biblioteci externe: fișiere (local sau server) sau baze de date
- obligatoriu (TBD) să integrați cel puțin două biblioteci externe pe lângă cele pentru stocare

### Tema 0

- [X] Nume proiect (poate fi schimbat ulterior)
- [X] Scurtă descriere a temei alese, ce v-ați propus să implementați

## Tema 1

#### Cerințe
<sumarry>

- [X] definirea a minim **3-4 clase** folosind compunere cu clasele definite de voi; moștenirile nu se iau în considerare aici
</sumarry>
<details>

[CurlSession](./include/CurlSession.h#L17)

[HTTPHeaders](./include/HTTPHeaders.h#L10)

[HTTPRequest](./include/HTTPRequest.h#L9) - has { [HTTPHeaders](./include/HTTPHeaders.h#L10) }, 

[HTTPResponse](./include/HTTPResponse.h#L7) - has { [HTTPHeaders](./include/HTTPHeaders.h#L10) }
</details>

- [X] constructori de inițializare cu parametri pentru fiecare clasă

[HTTPRequest](./src/HTTPRequest.cpp#L35)

[HTTPResponse](./src/HTTPResponse.cpp#L10)

[HTTPHeaders](./src/HTTPHeaders.cpp#L57)

[CurlSession](./include/CurlSession.h#L30)


- [X] pentru o aceeași (singură) clasă: constructor de copiere, `operator=` de copiere, destructor
<details>

Constructor de copiere [HTTPHeaders](./src/HTTPHeaders.cpp#L67)

Constructor de mutare [HTTPHeaders](./src/HTTPHeaders.cpp#L73)

Operator = de copiere [HTTPHeaders](./src/HTTPHeaders.cpp#L81)

Operator = de mutare [HTTPHeaders](./src/HTTPHeaders.cpp#L97)

Destructor [HTTPHeaders](./src/HTTPHeaders.cpp#L110)

</details>

- [X] `operator<<` pentru **toate** clasele pentru afișare (`std::ostream`) folosind compunere de apeluri cu `operator<<`

<details>

- [HTTPResponse](./src/HTTPResponse.cpp#L27)

- [HTTPRequest](./src/HTTPRequest.cpp#L87)

- [HTTPHeaders](./src/HTTPHeaders.cpp#L117)

- [CurlSession](./src/CurlSession.cpp#L159)

</details>


- [X] cât mai multe `const` (unde este cazul) și funcții `private`

<details>

~40 const

[HTTPHeaders::format_header](./src/HTTPHeaders.cpp#L14)

[HTTPResponse::sync_with_raw_body](./src/HTTPResponse.cpp#L3)

[HTTPRequest::parse_status_line](./src/HTTPRequest.cpp#L10)

[HTTPRequest::parse_status_line](./src/HTTPRequest.cpp#L10)

[HTTPRequest::parse_header](./src/HTTPRequest.cpp#L22)

[CurlSession::getinfo_wrapper_last_url](./src/CurlSession.cpp#L17)

</details>

- [X] implementarea a minim 3 funcții membru publice pentru funcționalități netriviale specifice temei alese, dintre care cel puțin 1-2 funcții mai complexe
  - nu doar citiri/afișări sau adăugat/șters elemente într-un/dintr-un vector

<details>

Cam tot ce e aici e complex, dar uite:

[CurlSession::request](./src/CurlSession.cpp#L35) <- Cea mai complexa

[HTTPRequest::perform](./src/HTTPRequest.cpp#L74)

[HTTPHeaders::get_data](./src/HTTPHeaders.cpp#L19)

</details>

- [X] scenariu de utilizare **cu sens** a claselor definite:
  - crearea de obiecte și apelarea tuturor funcțiilor membru publice în main
  - vor fi adăugate în fișierul `tastatura.txt` DOAR exemple de date de intrare de la tastatură (dacă există); dacă aveți nevoie de date din fișiere, creați alte fișiere separat
- [X] minim 52-60% din codul propriu să fie C++, `.gitattributes` configurat corect
- [X] tag de `git`: de exemplu `v0.1`
- [X] serviciu de integrare continuă (CI) cu **toate bifele**; exemplu: GitHub Actions
- [X] code review #1 2 proiecte

## Tema 2

#### Cerințe
- [X] separarea codului din clase în `.h` (sau `.hpp`) și `.cpp`
- [X] moșteniri:
  - minim o clasă de bază și **3 clase derivate** din aceeași ierarhie; cele 3 derivate moștenesc aceeași clasă de bază
  - ierarhia trebuie să fie cu bază proprie, nu derivată dintr-o clasă predefinită
  - [X] funcții virtuale (pure) apelate prin pointeri de bază din clasa care conține atributul de tip pointer de bază
    - minim o funcție virtuală va fi **specifică temei** (i.e. nu simple citiri/afișări sau preluate din biblioteci i.e. draw/update/render)
    - constructori virtuali (clone): sunt necesari, dar nu se consideră funcții specifice temei
    - afișare virtuală, interfață non-virtuală
  - [X] apelarea constructorului din clasa de bază din constructori din derivate
  - [X] clasă cu atribut de tip pointer la o clasă de bază cu derivate; aici apelați funcțiile virtuale prin pointer de bază, eventual prin interfața non-virtuală din bază
    - [X] suprascris cc/op= pentru copieri/atribuiri corecte, copy and swap
    - [X] `dynamic_cast`/`std::dynamic_pointer_cast` pentru downcast cu sens
    - [X] smart pointers (recomandat, opțional)
- [X] excepții
  - [X] ierarhie proprie cu baza `std::exception` sau derivată din `std::exception`; minim **3** clase pentru erori specifice distincte
    - clasele de excepții trebuie să trateze categorii de erori distincte (Network Client vs HTTP Status Line vs HTTP Headers)
  - [X] utilizare cu sens: de exemplu, `throw` în constructor (sau funcție care întoarce un obiect), `try`/`catch` în `main`
  - această ierarhie va fi complet independentă de ierarhia cu funcții virtuale
implementate in [Exceptions.hpp](./include/Exceptions.hpp) si folosite in [CurlSession](./src/CurlSession.cpp) si [HTTPRequest](./src/HTTPRequest.cpp)

<details>
<summary>Referințe Tema 2 (Moșteniri, Smart Pointers, etc.)</summary>

[StrategyRegistry::strategies folosind std::unique_ptr](./include/StrategyRegistry.h#L18)

[StrategyRegistry::runOnly folosind dynamic_cast pentru downcast cu sens](./include/StrategyRegistry.h#L58)

[Clase derivate: SsjsTimeBlindStrategy, SsjsBooleanBlindStrategy, OperatorInjectionStrategy](./include/strategies/)
</details>

- [X] funcții și atribute `static`

<details>

Funcții membre `static`:

[HTTPHeaders::normalize](./include/HTTPHeaders.h#L16) implementat [aici](./src/HTTPHeaders.cpp#L10)

[HTTPHeaders::format_header](./include/HTTPHeaders.h#L17) implementat [aici](./src/HTTPHeaders.cpp#L14)

Variabile locale `static`:

[HTTPRequest::parse_status_line - static std::regex pattern](./src/HTTPRequest.cpp#L12)

[HTTPHeaders::operator[] - static const std::string empty_string](./src/HTTPHeaders.cpp#L47) - necesar că vreau să returnez o referință

</details>

- [X] STL
<details>
<summary>Referințe STL</summary>

[HTTPHeaders::headers folosind std::unordered_map](./include/HTTPHeaders.h#L11)

[HTTPResponse::raw_body folosind std::vector](./include/HTTPResponse.h#L14)

[CurlSession::request / HTTPRequest::perform folosind std::expected](./include/CurlSession.h#L25)

[HTTPRequest::parse_status_line și parse_header folosind std::regex și std::smatch](./src/HTTPRequest.cpp#L10)

</details>
- [X] cât mai multe `const` (deja rezolvat în Milestone 1)
- [X] funcții *de nivel înalt*, de eliminat cât mai mulți getters/setters/funcții low-level
<details>
<summary>Referințe funcții de nivel înalt</summary>

[HTTPRequest::HTTPRequest(std::istream&)](./src/HTTPRequest.cpp#L35) - parsează și construiește un request complet din fluxul de intrare

[HTTPRequest::perform](./src/HTTPRequest.cpp#L74) - executa un HTTPRequest folosind CURL

[CurlSession::request](./src/CurlSession.cpp#L35) - orchestrează request-ul complet către libcurl și construiește răspunsul

</details>
- [X] minim 75-78% din codul propriu să fie C++

- [X] la sfârșit: commit separat cu adăugarea unei noi clase derivate fără a modifica restul codului, **pe lângă cele 3 derivate deja adăugate** din aceeași ierarhie
  - noua derivată nu poate fi una existentă care a fost ștearsă și adăugată din nou
  - noua derivată va fi integrată în codul existent (adică va fi folosită, nu adăugată doar ca să fie) - commit 1fed66676c62e2e38f2869bf48134ceeffed5de8

- [ ] tag de `git` pe commit cu **toate bifele**: de exemplu `v0.2`

- [ ] code review #2 2 proiecte

## Tema 3

#### Cerințe
- [X] 2 șabloane de proiectare (design patterns)
<details>
<summary>Referințe Design Patterns</summary>

1. [Strategy Pattern: Base Strategy](./include/strategies/Strategy.h) și clasele derivate.
2. [Builder Pattern: StrategyResultBuilder](./include/StrategyResult.h#L41)
3. [Singleton Pattern: Strategy Registry](./include/StrategyRegistry.h#L8)
</details>

- [X] o clasă șablon cu sens; minim **2 instanțieri**
<details>
<summary>Referințe Clasă Șablon (Declarare și Utilizare)</summary>

- Declarare: [Pipeline<T>](./include/Pipeline.h#L8)
- Instanțiere 1: [Pipeline<std::string>](./include/ResponseDiffEngine.h#L14)
- Instanțiere 2: [Pipeline<HTTPResponse>](./include/ResponseDiffEngine.h#L15)
</details>

  - [X] preferabil și o funcție șablon (template) cu sens; minim 2 instanțieri
<details>
<summary>Referințe Funcție Șablon</summary>

1. [concat (Variadic Template)](./include/Utilities.h#L44)
</details>

- [X] minim 80-90% din codul propriu să fie C++
<!-- - [ ] o specializare pe funcție/clasă șablon -->
- [ ] tag de `git` pe commit cu **toate bifele**: de exemplu `v0.3` sau `v1.0`
- [ ] code review #3 2 proiecte

## Instrucțiuni de compilare

Proiectul este configurat cu CMake.

Instrucțiuni pentru terminal:

1. Pasul de configurare
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
# sau ./scripts/cmake.sh configure
```

Sau pe Windows cu GCC folosind Git Bash:
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G Ninja
# sau ./scripts/cmake.sh configure -g Ninja
```

Pentru a configura cu ASan, avem opțiunea `-DUSE_ASAN=ON` (nu merge pe Windows cu GCC):
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DUSE_ASAN=ON
# sau ./scripts/cmake.sh configure -e "-DUSE_ASAN=ON"
```


La acest pas putem cere să generăm fișiere de proiect pentru diverse medii de lucru.


2. Pasul de compilare
```sh
cmake --build build --config Debug --parallel 6
# sau ./scripts/cmake.sh build
```

Cu opțiunea `parallel` specificăm numărul de fișiere compilate în paralel.


3. Pasul de instalare (opțional)
```sh
cmake --install build --config Debug --prefix install_dir
# sau ./scripts/cmake.sh install
```

Vezi și [`scripts/cmake.sh`](scripts/cmake.sh).

Observație: folderele `build/` și `install_dir/` sunt adăugate în fișierul `.gitignore` deoarece
conțin fișiere generate și nu ne ajută să le versionăm.


## Instrucțiuni pentru a rula executabilul

Există mai multe variante:

1. Din directorul de build (implicit `build`). Executabilul se află la locația `./build/oop` după ce a fost rulat pasul de compilare al proiectului (`./scripts/cmake.sh build` - pasul 2 de mai sus).

```sh
./build/oop
```

2. Din directorul `install_dir`. Executabilul se află la locația `./install_dir/bin/oop` după ce a fost rulat pasul de instalare (`./scripts/cmake.sh install` - pasul 3 de mai sus).

```sh
./install_dir/bin/oop
```

3. Rularea programului folosind Valgrind se poate face executând script-ul `./scripts/run_valgrind.sh` din rădăcina proiectului. Pe Windows acest script se poate rula folosind WSL (Windows Subsystem for Linux). Valgrind se poate rula în modul interactiv folosind: `RUN_INTERACTIVE=true ./scripts/run_valgrind.sh`

Implicit, nu se rulează interactiv, iar datele pentru `std::cin` sunt preluate din fișierul `tastatura.txt`.

```sh
RUN_INTERACTIVE=true ./scripts/run_valgrind.sh
# sau
./scripts/run_valgrind.sh
```

4. Pentru a rula executabilul folosind ASan, este nevoie ca la pasul de configurare (vezi mai sus) să fie activat acest sanitizer. Ar trebui să meargă pe macOS și Linux. Pentru Windows, ar merge doar cu MSVC (nerecomandat).

Comanda este aceeași ca la pasul 1 sau 2. Nu merge combinat cu Valgrind.

```sh
./build/oop
# sau
./install_dir/bin/oop
```

## License

The project is licensed under [AGPLv3](LICENSE).

The [template repository](https://github.com/mcmarius/oop-template) itself is licensed under [Unlicense](LICENSE.template).

## Resurse

- adăugați trimiteri **detaliate** către resursele externe care v-au ajutat sau pe care le-ați folosit
