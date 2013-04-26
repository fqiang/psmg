#ifndef CHANGEITEM_H_
#define CHANGEITEM_H_



/** @struct changeitem
 *  Simple struct that stores a queued change to the model tree.
 *
 *  This is needed to treat expectation constraints that in the postprocessing
 *  need to be removed from the model in which they are defined and added
 *  to a different model. This action cannot be done by recursively working
 *  through all models and ModelComps (since removing/adding comps
 *  invalidates the iterators used in the recursion)
 */
enum CHANGE_TYPE {CHANGE_NOACT=0,CHANGE_REM=1,CHANGE_ADD=2};

typedef struct changeitem {


  /** The component to be added or removed */
  ModelComp *comp;

  /** The model to which it should be added/removed */
  AmplModel *model;

  /** The action: CHANGE_REM/CHANGE_ADD */
  CHANGE_TYPE action;

} changeitem;

#endif
