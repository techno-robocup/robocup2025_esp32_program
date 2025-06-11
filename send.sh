find . -name "*.cpp" -o -name "*.h" -o -name "*.c" -o -name "*.hpp" | while read file; do
    rsync -av --delete --exclude .direnv/ --exclude .git/ "$file" robo@roboberry.local:robocup2025_esp32_program/$file
done

