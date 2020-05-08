# Example
Небольшой репозиторий для демонстрации моих умений (work in progress).
Для установки зависимостей не забываем про `npm install` :)

**1. Native Modules**

В папке `./src/emitter/` содержится исходный код, который позволяет вызывать/получать события из нативного модуля в JS-код и обратно. Для построения из исходного кода нужно установить  [Native Abstractions for Node.js](https://github.com/nodejs/nan) и выполнить `npm run build`, после чего в папке `build/Release/` должен повиться собраный модуль (маска для имени файла `*.node`). В папке `./test` находится прекомпилированый модуль для Windows x64 и небольшой интеграционный тест.