#!/bin/bash

pre-commit install

pre-commit clean

{
cat << 'EOF'
#!/bin/bash

#
# C Format and Lint
#

echo "Start: C Format and Lint"

echo "Execute: clang-format"
c_format_all() {
    alias c-format-all="find . -path ./build -prune -o -name \"*.c\" -print -exec clang-format -i {} +"
}

c_format_all

echo "Execute: clang-tidy"
c_lint_all() {
    alias c-lint-all='find src -name "*.c" -exec clang-tidy {} -header-filter=.* -system-headers=false -p build --checks=* \;'
}

c_lint_all

if [ $? -ne 0 ]; then
    echo "Error: clang-tidy failed."
    exit 1
fi

echo "Complete: C Format and Lint"

#
# Pre-Commit
#

EOF

cat .git/hooks/pre-commit

} > .git/hooks/pre-commit.tmp

mv .git/hooks/pre-commit.tmp .git/hooks/pre-commit

sudo chmod +x .git/hooks/pre-commit

echo "Success: Appended C format and lint"
