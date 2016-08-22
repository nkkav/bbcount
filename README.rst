=====================
 bbcount user manual
=====================

+-------------------+----------------------------------------------------------+
| **Title**         | bbcount (Basic block counter annotation pass for         |
|                   | Machine-SUIF)                                            |
+-------------------+----------------------------------------------------------+
| **Author**        | Nikolaos Kavvadias 2004, 2005, 2006, 2007, 2008, 2009    |
|                   | 2010, 2011, 2012, 2013, 2014, 2015, 2016                 |
+-------------------+----------------------------------------------------------+
| **Contact**       | nikos@nkavvadias.com                                     |
+-------------------+----------------------------------------------------------+
| **Website**       | http://www.nkavvadias.com                                |
+-------------------+----------------------------------------------------------+
| **Release Date**  | 02 October 2014                                          |
+-------------------+----------------------------------------------------------+
| **Version**       | 1.0.0                                                    |
+-------------------+----------------------------------------------------------+
| **Rev. history**  |                                                          |
+-------------------+----------------------------------------------------------+
|        **v1.0.0** | 2004-10-02                                               |
|                   |                                                          |
|                   | Initial release.                                         |
+-------------------+----------------------------------------------------------+


1. Introduction
===============

``bbcount`` is an annotation pass built to be used with the SUIF2/MachSUIF2 
compiler infrastructure. This pass creates a basic block annotation (BbNote) 
that is attached to the first machine instruction of basic block. BbNote 
information can then be exported to text files for diagnostic and other uses.
A BbNote can then be read by subsequent passes.

The BbNote class extends Machine-SUIF class Note and provides the following 
methods:

| ``int get_idnum() const``

Returns the first member of the note (procedure/CFG identification number).

| ``void set_idnum(int idnum)``

Sets the first member of the note to ``idnum``.

| ``IdString get_bb_counter() const``

Returns the basic block counter (second member) as a string.

| ``void set_bb_counter(IdString idbb)``.

Sets the basic block counter to the ``idbb`` string contents.

This pass uses the machine and cfg libraries. What it actually generates are the
the values of BbNote annotations in a text file (``bb_counters.txt``). The format
of this file is shown below:

::

  cnt_exec_freq.<proc_count>.<bb_num>  ;; once for each basic block
  ...
  ...
  <proc_name> <proc_count> <bb_num-1>  ;; once at the end of a CFG   

where:

proc_name:
  is the string representation of the procedure's name
proc_count:
  the absolute enumeration of the procedure in the translation unit
bb_num:
  the absolute enumeration of the basic block in the given CFG/procedure.

This pass works for the SUIFvm instruction set as well as other MachSUIF 
backends. The ``bbcount`` pass has been tested with MachSUIF 2.02.07.15.


2. File listing
===============

The ``bbcount`` distribution includes the following files:
   
+-----------------------+------------------------------------------------------+
| /bbcount              | Top-level directory                                  |
+-----------------------+------------------------------------------------------+
| AUTHORS               | List of ``bbcount`` authors.                         |
+-----------------------+------------------------------------------------------+
| LICENSE               | The modified BSD license governs ``bbcount``.        |
+-----------------------+------------------------------------------------------+
| README.rst            | This file.                                           |
+-----------------------+------------------------------------------------------+
| README.html           | HTML version of README.                              |
+-----------------------+------------------------------------------------------+
| README.pdf            | PDF version of README.                               |
+-----------------------+------------------------------------------------------+
| VERSION               | Current version of the project sources.              |
+-----------------------+------------------------------------------------------+
| bbcount.cpp           | Implementation of the ``bbcount`` pass.              |
+-----------------------+------------------------------------------------------+
| bbcount.h             | C++ header file containing declarations and          |
|                       | prototypes for the above.                            |
+-----------------------+------------------------------------------------------+
| rst2docs.sh           | Bash script for generating the HTML and PDF versions |
|                       | of the documentation (README).                       |
+-----------------------+------------------------------------------------------+
| suif_main.cpp         | Entry point for building the standalone program      |
|                       | ``do_bbcount`` that implements the pass.             |
+-----------------------+------------------------------------------------------+
| suif_pass.cpp         | Define the SUIF pass built as the dynamically        |
|                       | loadable library ``libbbcount.so``.                  |
+-----------------------+------------------------------------------------------+
| suif_main.h           | C++ header file for the above.                       |
+-----------------------+------------------------------------------------------+
| utils.h               | C header file with implementations of auxiliary      |
|                       | functions.                                           |
+-----------------------+------------------------------------------------------+


3. Installation
===============

Unpack the ``bbcount`` archive wherever you like, e.g. in ``$MACHSUIFHOME/cfa/bbcount``.
You don't need to modify anything in the Makefile, if you have a working
MachSUIF 2 installation.

The program binary (``do_bbcount``) will be installed at ``$NCIHOME/bin`` and 
the shared library (``libbbcount.so``) at ``$NCIHOME/solib``, where NCIHOME is 
the SUIF 2 top-level directory.


4. Usage details
================

The pass accepts an input file in CFG form to operate. Textual output is 
generated, written to stdout by default.

Usage synopsys:
| ``$ do_bbcount test.cfg``
