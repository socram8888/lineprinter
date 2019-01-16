lineprinter
===========

Tiny test tool for sending raw [ESC/POS](https://reference.epson-biz.com/modules/ref_escpos/) to thermal printers under Windows.

Usage
-----

`lineprinter -p [printer name] -f [text file]`

The printer name is mandatory, and can be found in the printers section of Control Panel. It must be specified as it appears, with spaces if any.

The file is optional, and if unspecified, it will read from standard input.
