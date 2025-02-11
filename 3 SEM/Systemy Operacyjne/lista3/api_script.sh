#!/bin/bash

# Funkcja pobierająca obraz z The Cat API
get_cat_image() {
    local cat_api_url="https://api.thecatapi.com/v1/images/search"
    local image_url=$(curl -s "$cat_api_url" | jq -r '.[0].url')
    
    if [[ -z "$image_url" ]]; then
        echo "Nie udało się pobrać obrazu kota."
        exit 1
    fi

    # Pobranie obrazu tymczasowo do pliku
    curl -s "$image_url" -o /tmp/cat_image.jpg
    
    # Wyświetlenie obrazu w terminalu
    catimg /tmp/cat_image.jpg
}

# Funkcja pobierająca cytat Chucka Norrisa z API
get_chuck_norris_quote() {
    local chuck_api_url="https://api.chucknorris.io/jokes/random"
    local quote=$(curl -s "$chuck_api_url" | jq -r '.value')
    
    if [[ -z "$quote" ]]; then
        echo "Nie udało się pobrać cytatu Chucka Norrisa."
        exit 1
    fi

    # Wyświetlenie cytatu
    echo -e "\nCytat Chucka Norrisa:"
    echo "$quote"
}

# Główna funkcja programu
main() {
    get_cat_image
    get_chuck_norris_quote
}

# Uruchomienie programu
main
