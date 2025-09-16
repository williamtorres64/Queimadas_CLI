#!/bin/bash

SOURCE_DIR="csv_bruto"
OUTPUT_FILE="$SOURCE_DIR/merged_focos.csv"

mkdir -p "$SOURCE_DIR"

if [ ! -d "$SOURCE_DIR" ]; then
    echo "Error: The directory '$SOURCE_DIR' was not found."
    echo "Please make sure you are running this script from the correct location."
    exit 1
fi

echo "Starting the merge process..."

FIRST_FILE=$(find "$SOURCE_DIR" -type f -name "*.csv" -not -name "$(basename "$OUTPUT_FILE")" | head -n 1)

if [ -z "$FIRST_FILE" ]; then
    echo "No CSV files found in '$SOURCE_DIR' to merge."
    exit 1
fi

echo "Using the header from: $FIRST_FILE"
head -n 1 "$FIRST_FILE" > "$OUTPUT_FILE"

echo "Merging data from all files..."
find "$SOURCE_DIR" -type f -name "*.csv" -not -name "$(basename "$OUTPUT_FILE")" -print0 | while IFS= read -r -d '' file; do
    if [ "$file" != "$FIRST_FILE" ]; then
        tail -n +2 "$file" >> "$OUTPUT_FILE"
    fi
done

echo ""
echo "Merge complete! All data has been combined into '$OUTPUT_FILE'."
echo "The final file can be found at: $OUTPUT_FILE"

