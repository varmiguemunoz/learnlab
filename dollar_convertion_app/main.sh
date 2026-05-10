#!/usr/bin/env bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

get_canva_design() {
  if [[ ! -f "$SCRIPT_DIR/rose_design.txt" ]]; then
    echo "Design file not found!"
    exit 1
  fi

  cat "$SCRIPT_DIR/rose_design.txt"
}

get_canva_design

echo "Hi there, welcome to the dollar app convertion"

read -p "How much money you got? " dollar_input_value

if [[ -z "$dollar_input_value" ]]; then
  echo "Please insert a number"
elif ! [[ "$dollar_input_value" =~ ^[0-9]+$ ]]; then
  echo "Please enter a valid number"
  exit 1
fi

get_dollar_value() {
  # TODO: Get the real value of the dollar from an API
  echo "35000"

}

main() {
  local current_dollar_value="$1"
  local user_input_value="$2"

  local conversion_result=$(echo "$user_input_value * $current_dollar_value" | bc -l)

  echo "Total amount on COP: $conversion_result"
}

current_dollar_value="$(get_dollar_value)"

main "$current_dollar_value" "$dollar_input_value"

should_continue="y"

while [["$should_continue" == "y"]]; do
  read -p "Do you want to convert another amount? (y/n) " answer
  case "$answer" in
  [Yy]*)
    read -p "How much money you got? " dollar_input_value

    if [[ -z "$dollar_input_value" ]]; then
      echo "Please insert a number"
    elif ! [[ "$dollar_input_value" =~ ^[0-9]+$ ]]; then
      echo "Please enter a valid number"
      exit 1
    else
      main "$current_dollar_value" "$dollar_input_value"
    fi
    ;;
  [Nn]*)
    should_continue="n"
    echo "Goodbye!"
    exit 0
    ;;
  *)
    echo "Please answer y or n."
    ;;
  esac
done
