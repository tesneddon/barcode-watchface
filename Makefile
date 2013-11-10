.PHONY: all

all: js/modernizr-2.6.2.min.js
	compass compile
	jekyll build

js/modernizr-2.6.2.min.js: bower_components/gumby/js/libs/modernizr-2.6.2.min.js
	cp $< $@

