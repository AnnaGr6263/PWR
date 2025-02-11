#!/bin/bash

# Pętla iteruje przez wszystkie pliki
for FILE in *; do
    # Sprawdzenie, czy jest to plik czyli pomijamy podkatalogi
    if [ -f "./$FILE" ]; then
        # Zamieniamy na male litery
        NEW_NAME=$(echo "$FILE" | tr '[:upper:]' '[:lower:]')
        # Sprawdzenie, czy nazwa wymaga zmiany
        if [ "$FILE" != "$NEW_NAME" ]; then
            mv -- "./$FILE" "./$NEW_NAME"  # uzywam -- aby nie bylo problemu z plikami zaczynajacymi się od '-'
        fi
    fi
done
