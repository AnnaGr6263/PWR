package pl.library;

/**
 * Klasa Copy reprezentuje pojedynczy egzemplarz książki.
 * Zastosowanie GRASP: High Cohesion - Klasa Copy jest odpowiedzialna wyłącznie za stan dostępności egzemplarza.
 */
public final class Copy {

    /** Książka, do której należy egzemplarz. */
    private final Book book;

    /** Stan dostępności egzemplarza. */
    private boolean isAvailable;

    /**
     * Tworzy nowy egzemplarz książki.
     *
     * @param parentBook obiekt Book, do którego należy egzemplarz
     */
    public Copy(final Book parentBook) {
        this.book = parentBook;
        this.isAvailable = true;
    }

    /**
     * Sprawdza, czy egzemplarz jest dostępny.
     *
     * @return true, jeśli egzemplarz jest dostępny, false w przeciwnym razie.
     */
    public boolean isAvailable() {
        return isAvailable;
    }

    /**
     * Ustawia egzemplarz jako wypożyczony.
     * Zastosowanie GRASP: Information Expert - Copy zarządza swoim stanem dostępności.
     */
    public void borrow() {
        this.isAvailable = false;
    }

    /**
     * Ustawia egzemplarz jako oddany czyli znowu dostępny.
     * Zastosowanie GRASP: Information Expert - Copy zarządza swoim stanem dostępności.
     */
    public void returnCopy() {
        this.isAvailable = true;
    }

    /**
     * Zwraca tytuł książki, do której należy egzemplarz.
     *
     * @return tytuł książki.
     */
    public String getBookTitle() {
        return book.getTitle();
    }
}
