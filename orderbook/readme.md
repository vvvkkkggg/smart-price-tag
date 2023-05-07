# Отчёт по задаче Orderbook

Я довольно поздно выяснил, что если Linux в Docker-контейнере или VM, то cache loads и cache misses (а может быть и всё остальное) считаются некорректно. К сожалению, установить на мой ноутбук Linux у меня нет возможности. Но репорты всё равно приложу, хотя бы в сравнении видно разницу между реализациями.

###### std::map with static allocator

std::map, предсказуемо, оказался худшим по количеству branch misprediction и вообще пол количеству циклов инструкций.

![](/Users/romangostilo/Library/Application%20Support/marktext/images/2023-05-08-05-46-32-image.png)

###### abseil::btree_map with static allocator

B-дерево оказалось выполнило задачу за более чем вдвое меньшее число циклов и инструкций, нежели map из stl

![](/Users/romangostilo/Library/Application%20Support/marktext/images/2023-05-08-05-48-58-image.png)

###### circullar buffer

Неожиданно, абсолютным фаворитом оказался circullar buffer, обогнав по количеству циклов и инструкций B-дерево в 10 раз, и в два раза реже ошибаясь при выборе ветки.

Но на самом деле, в задаче просто почти не задействована операция, которая занимает довольно длительное время у этой реализации - итерация по ордербуку.

![](/Users/romangostilo/Library/Application%20Support/marktext/images/2023-05-08-05-50-12-image.png)
