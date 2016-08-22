/* file "bbcount/suif_pass.cpp" */
/*
 *     Copyright (c) 2004, 2005, 2006, 2007, 2008, 2009, 2010,
 *                   2011, 2012, 2013, 2014, 2015, 2016 Nikolaos Kavvadias
 *
 *     This software was written by Nikolaos Kavvadias, Ph.D. candidate
 *     at the Physics Department, Aristotle University of Thessaloniki,
 *     Greece (at the time).
 *
 *     This software is provided under the terms described in
 *     the "machine/copyright.h" include file.
 */

#include <machine/copyright.h>

#ifdef USE_PRAGMA_INTERFACE
#pragma implementation "bbcount/suif_pass.h"
#endif

#include <machine/pass.h>
#include <machine/machine.h>
#include <cfg/cfg.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#define new D_NEW
#endif

#include <bbcount/suif_pass.h>

IdString k_bb;

extern "C" void
init_bbcount(SuifEnv *suif_env)
{
    static bool init_done = false;

    if (init_done)
	return;
    init_done = true;

    k_bb = "bb";

    ModuleSubSystem *mSubSystem = suif_env->get_module_subsystem();
    mSubSystem->register_module(new BbcountSuifPass(suif_env));

    // initialize the libraries required by this OPI pass
    init_suifpasses(suif_env);
    init_machine(suif_env);
    init_cfg(suif_env);
}

BbcountSuifPass::BbcountSuifPass(SuifEnv *suif_env, const IdString& name)
    : PipelinablePass(suif_env, name)
{
    the_suif_env = suif_env;	// bind suif_env into our global environment
}

BbcountSuifPass::~BbcountSuifPass()
{
    // empty
}

void
BbcountSuifPass::initialize()
{
    PipelinablePass::initialize();

    // Create grammar for parsing the command line.  This must occur
    // after the parent class's initialize routine has been executed.
    _command_line->set_description("insert BB labels");

    // zero or more file names
    file_names = new OptionString("file name");
    OptionLoop *files =
	new OptionLoop(file_names,
		       "names of optional input and/or output files");
    _command_line->add(files);
}

bool
BbcountSuifPass::parse_command_line(TokenStream *command_line_stream)
{
    o_fname = empty_id_string;

    if (!PipelinablePass::parse_command_line(command_line_stream))
	return false;

    o_fname = process_file_names(file_names);

    return true;
}

void
BbcountSuifPass::execute()
{
    PipelinablePass::execute();

    // Process the output file name, if any.
    if (!o_fname.is_empty())
        the_suif_env->write(o_fname.chars());
}

void
BbcountSuifPass::do_file_set_block(FileSetBlock *fsb)
{
    claim(o_fname.is_empty() || fsb->get_file_block_count() == 1,
	  "Command-line output file => file set must be a singleton");

    set_opi_predefined_types(fsb);
}

void
BbcountSuifPass::do_file_block(FileBlock *fb)
{
    claim(has_note(fb, k_target_lib),
	  "expected target_lib annotation on file block");

    focus(fb);
    bbcount.initialize();
}

void
BbcountSuifPass::do_procedure_definition(ProcedureDefinition *pd)
{
    focus(pd);
    bbcount.do_opt_unit(pd);
    defocus(pd);
}

void
BbcountSuifPass::finalize()
{
    bbcount.finalize();
}
