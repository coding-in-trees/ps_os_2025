# Use this file at your own risk.
# Always ensure that the required files are actually put into the archive,
# and that no additional files are archived.
# You can use the zipinfo command to check the file structure of an existing archive.

USERNAME = csbb9638
EXERCISE = 11

# This excludes some common directories automatically.
# Adjust this list according to your setup.
# The zip command will also show which files are put into the archive.
# Check this output to ensure that only the required files are included.
EXCLUDE_PATTERNS = "README.md" "**.tar" "**.vscode/*" "**.idea/*" "**__MACOSX/*" "**.DS_Store/*" "**.dSYM/*" "task_1/secret"

ARCHIVE= "../exc$(EXERCISE)_$(USERNAME).zip"

.PHONY: all
all: zip

.PHONY: zip
zip: clean
	$(RM) $(ARCHIVE)
	zip -r $(ARCHIVE) . --exclude $(EXCLUDE_PATTERNS)

.PHONY: clean
clean:
	@for dir in task_?; do \
	  if [ -f "$$dir/Makefile" ] || [ -f "$$dir/makefile" ]; then \
	    $(MAKE) -C "$$dir" clean; \
	  fi \
	done
