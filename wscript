
#
# This file is the default set of rules to compile a Pebble project.
#
# Feel free to customize this to your needs.
#

top = '.'
out = 'build'

def options(ctx):
    ctx.load('pebble_sdk')

def configure(ctx):
    ctx.load('pebble_sdk')

def build(ctx):
    ctx(rule='cat ${SRC} > ${TGT}',
        source=ctx.path.ant_glob('src/js/*.js', excl=['pebble-js-app.js']),
        target='src/js/pebble-js-app.js')

    ctx.load('pebble_sdk')

    ctx.pbl_program(source=ctx.path.ant_glob('src/**/*.c'),
                    target='pebble-app.elf')

    ctx.pbl_bundle(elf='pebble-app.elf',
                   js=ctx.path.ant_glob('src/js/pebble-js-app.js'))

    print(ctx.path.ant_glob('src/js/*.js', excl=['pebble-js-app.js']))

                                                 
