Development
===========

[TOC]

# Working with the team

Generally it is sensible to check with the other developers if you are
planning to make a change to NetSurf intended to be merged.

We are often about on the IRC channel but failing that the developer
mailing list is a good place to try.

All the project sources are held in [public git repositories](http://source.netsurf-browser.org/)

# Compilation environment

Compiling a development edition of NetSurf requires a POSIX style
environment. Typically this means a Linux based system although Free
BSD, Open BSD, Mac OS X and Haiku all known to work.

## Toolchains

Compilation for non POSIX toolkits/frontends (e.g. RISC OS) generally
relies upon a cross compilation environment which is generated using
the makefiles found in our
[toolchains](http://source.netsurf-browser.org/toolchains.git/)
repository. These toolchains are built by the Continuous Integration
(CI) system and the
[results of the system](http://ci.netsurf-browser.org/builds/toolchains/)
are published as a convenience.

## Quick setup

The [quick start guide](docs/quick-start.md) can be used to get a
development environment setup quickly and uses the
[env.sh](env_8sh_source.html) script the core team utilises.

## Manual setup

The Manual environment setup and compilation method is covered by the
details in the [netsurf libraries](docs/netsurf-libraries.md) document
for the core libraries and then one of the building documents for the
specific frontend.

- [Amiga Os cross](docs/building-AmigaCross.md) and [Amiga OS](docs/building-AmigaOS.md)
- [Framebuffer](docs/building-Framebuffer.md)
- [GTK](docs/building-GTK.md)
- [Haiku (BeOS)](docs/building-Haiku.md)
- [Windows Win32](docs/building-Windows.md)

These documents are sometimes not completely up to
date and the env.sh script should be considered canonical.

# Logging

The [logging](docs/logging.md) interface controls debug and error
messages not output through the GUI.

# Unit testing

NetSurf [unit tests](docs/unit-testing.md) provide basic test coverage
of many core parts of the browser code such as url parsing and utility
functions.

# Integration testing

The monkey frontend is used to perform complex tests involving
operating the browser as a user might (opening windows, navigating to
websites and rendering the contents etc.)

A test is written as a set of operations in a yaml file. A test can be
run using the monkey_driver.py python script

    $ ./test/monkey_driver.py -m ./nsmonkey -t test/monkey-tests/start-stop.yaml

There are very few tests within the netsurf repository. The large
majority of integration tests are held within the
[netsurf-test](http://source.netsurf-browser.org/netsurf-test.git/)
repository.

To allow more effective use of these tests additional infrastructure
has been constructed to allow groupings of tests to be run. This is
used extensively by the CI system to perform integration testing on
every commit.

Each test is a member of a group and the tests within each group are
run together. Groups are listed within division index files. To run
the integration tests the monkey-see-monkey-do python script is
used. It downloads the test plan for a division from the netsurf test
infrastructrure and executes it.

    $ ./test/monkey-see-monkey-do
    Fetching tests...
    Parsing tests...
    Running tests...
    Start group: initial
      [ Basic checks that the browser can start and stop ]
      => Run test: start-stop-no-js.yaml
      => Run test: basic-navigation.yaml
      => Run test: start-stop.yaml
    Start group: no-networking
      [ Tests that require no networking ]
      => Run test: resource-scheme.yaml
    Start group: ecmascript
      [ ECMAScript tests ]
    PASS


# Documented API

The NetSurf code makes use of Doxygen for code documentation.

There are several documents which detail specific aspects of the
codebase and APIs.

## Core window

The [core window API](docs/core-window-interface.md) allows frontends
to use generic core code for user interface elements beyond the
browser render.

## Source object caching

The [source object caching](docs/source-object-backing-store.md)
provides a way for downloaded content to be kept on a persistent
storage medium such as hard disc to make future retrieval of that
content quickly.

# Javascript

Javascript provision is split into four parts:
- An engine that takes source code and executes it.
- Interfaces between the program and the web page.
- Browser support to retrieve and manage the source code to be executed.
- Browser support for the dispatch of events from user interface.

## Library

JavaScript is provided by integrating the duktape library. There are
[instructions](docs/updating-duktape.md) on how to update the library.

## Interface binding

In order for javascript programs to to interact with the page contents
it must use the Document Object Model (DOM) and Cascading Style Sheet
Object Model (CSSOM) API.

These interfaces are described using web Interface Description
Language (IDL) within the relevant specifications
(e.g. https://dom.spec.whatwg.org/).

Each interface described by the webIDL must be bound (connected) to
the browsers internal representation for the DOM or CSS, etc. The
process of [writing bindings](docs/jsbinding.md) is ongoing.
