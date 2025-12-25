# matrixlib_practice (c++20)

учебный проект под практику: библиотека операций над плотными матрицами (до 100x100) + консольное приложение + юнит-тесты.

## структура

- `matrix/` — библиотека `matrixlib`
- `apps/console/` — консольное приложение `matrix_console`
- `tests/` — google test
- `docs/` — doxygen + техописание
- `examples/` — входные файлы

## сборка (linux/mac)

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

запуск консоли:
```bash
./build/matrix_console examples/input_3x3.txt
```

запуск тестов:
```bash
ctest --test-dir build --output-on-failure
```

## сборка с санитайзерами (задачи 8/10)

```bash
cmake -S . -B build-asan -DCMAKE_BUILD_TYPE=Debug -DMATRIXLIB_ENABLE_SANITIZERS=ON
cmake --build build-asan -j
./build-asan/matrix_console examples/input_100x100.txt
```

## формат входного файла

две матрицы подряд. пустая строка между матрицами допускается.

```
R C
... R строк по C чисел ...

R C
... R строк по C чисел ...
```

## операции в консоли

- `1` add (A+B)
- `2` sub (A-B)
- `3` scalar (A*k)
- `4` mul (A*B)
- `5` transpose (A^T)
- `6` trace (tr(A))
- `7` det (det(A), ограничение: n<=10)
- `8` solve (A x = b) — lu-решатель (b берется из второй матрицы как столбец)

результат печатается в консоль и сохраняется в `out.txt` рядом с входным файлом.
