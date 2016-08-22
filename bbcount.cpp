/* file "bbcount/bbcount.cpp" */
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
#pragma implementation "bbcount/bbcount.h"
#endif

#include <machine/machine.h>
#include <cfg/cfg.h>

#include <bbcount/bbcount.h>
#include "utils.h"

#ifdef USE_DMALLOC
#include <dmalloc.h>
#define new D_NEW
#endif

extern IdString k_bb;

class BbNote : public Note {
 public:
    BbNote() : Note(note_list_any()) { }
    BbNote(const BbNote &other) : Note(other) { }
    BbNote(const Note &note) : Note(note) { }

    int get_idnum() const              { return _get_c_long(0); }
    void set_idnum(int idnum)          { _replace(0, idnum); }
    IdString get_bb_counter() const    { return _get_string(1); }
    void set_bb_counter(IdString idbb) { _replace(1, idbb); }
};

void
Bbcount::do_opt_unit(OptUnit *unit)
{
  int bb_num = 0;           // Number of the current basic block
  char bb_num_s[25];        // String equivalent to bb_num
  char procedure_count_s[25];
  /**/
  // FILE to store the dependency matrices for all basic blocks
  FILE *f_bbcounters_file;
  /**/
  int mi_opcode;
  //
  static int procedure_count = 0;
  static int global_bb_num = 0;

  IdString name = get_name(get_proc_sym(unit));
  debug(1, "Processing procedure \"%s\"", name.chars());

  // get the body of the OptUnit
  AnyBody *orig_body = get_body(unit);

  if (is_kind_of<InstrList>(orig_body))
      return;		// nothing to do

  claim(is_kind_of<Cfg>(orig_body),
        "expected OptUnit body in Cfg form");

  // print the CFG if debugging verbosely
  if_debug(5)
      fprint(stderr, static_cast<Cfg*>(orig_body), false, true);

   // Open dependency matrix output file
  if (procedure_count==0)
    f_bbcounters_file = fopen("bb_counters.txt","w");
  else
    f_bbcounters_file = fopen("bb_counters.txt","a");

  // Report name of the CFG under processing
  const char *cur_proc_name = get_name(unit).chars(); /* was cur_unit */

  // Get the body of the OptUnit
  AnyBody *cur_body = get_body(unit); /* was cur_unit */

  // Create a local copy of the input CFG
  Cfg *cfg = (Cfg *)cur_body;

  canonicalize(cfg,true,true,false);
  remove_unreachable_nodes(cfg);
  merge_node_sequences(cfg);
  optimize_jumps(cfg);


  // Iterate through the nodes of the CFG
  for (CfgNodeHandle cfg_nh=start(cfg); cfg_nh!=end(cfg); ++cfg_nh)
  {
    // Get the current node
    CfgNode* cnode = get_node(cfg, cfg_nh);

    /****************************************************************/
    /********************* INSIDE A CFGNODE *************************/
    /****************************************************************/

    // Obtain bb_num_s alphanumeric representation of bb_num integer
    itoa(bb_num, bb_num_s);
    
    // Obtain procedure_count_s alphanumeric representation of procedure_count integer
    itoa(procedure_count, procedure_count_s);

    // Create the actual name of the adjmat identifier for the current cfgnode (BB)
    char *bb_counter_name = (new char[strlen(procedure_count_s) + strlen(bb_num_s) + 16]);

    // Print the string for "adjmat_name" e.g. classify_6_adjmat
    sprintf(bb_counter_name, "cnt_exec_freq.%s.%s", procedure_count_s, bb_num_s);

    if (size(cnode)!=0)
    {
      // Get the 1st instruction in the basic block (BB = CfgNode)
      InstrHandle mih_first = start(cnode);
      //Instr *mi_first = first_non_label(cnode);
      //
      Instr *mi_first = *mih_first;

      BbNote bb_note;
      bb_note.set_bb_counter(IdString(bb_counter_name));
      bb_note.set_idnum(global_bb_num);
      set_note(mi_first, k_bb, bb_note);

      // Examine note
      BbNote bb_note_read = get_note(mi_first, k_bb);
    }

    // The number of current basic block
    bb_num++;
    // Global basic block counter
    global_bb_num++;

    /****************************************************************/
    /****************** end of INSIDE A CFGNODE *********************/
    /****************************************************************/

  } // end of for each CFGnode

  fprintf(f_bbcounters_file,"%s %d %d\n", cur_proc_name, procedure_count, bb_num-1);
  procedure_count++;

  fclose(f_bbcounters_file);
}
