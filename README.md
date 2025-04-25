# Sudoku
Jest to projekt gry Sudoku w języku C.
Program posiada menu z opcjami takimi jak: nowa gra, wczytaj grę, instrukcja i wyjście.

Opcja nowa gra umożliwia wybór wielkości planszy, wybór trudności oraz wejście w tryb gry.
Tryb gry oferuje wpisywanie, usuwanie, podpowiadanie poprawnych wartości, gdy wpisana wartość jest niepoprawna, a także zakończenie gry, które wywołuje opcję zapisania gry oraz pokazuje czas gry. Wygranie gry jest możliwe tylko wtedy, gdy plansza jest pełna i rozwiązanie jest poprawne.

Opcja wczytaj grę umożliwia wejście do wcześniej zakończonej gry zapisanej w pliku save.txt, który sam się tworzy i potem nadpisuje.

Opcja instrukcja wyświetla instrukcję, która i tak jest pokazywana na samym początku gry.

Opcja wyjście umożliwia zakończenie działania programu.

Błędy z którymi się pojawiły, ale zostały rozwiązane: wpisywanie typu char powodowało zapętlenie się programu, po usuwaniu liczb z planszy nie dało się wygrać gry, generowanie planszy, żeby była czytelna, komunikaty ostrzegające przed potencjalnie niebezpiecznymi funkcjami.
