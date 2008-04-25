#include "compiler/rewriter/framework/rule_driver.h"
#include "compiler/rewriter/rules/ruleset.h"
#include "compiler/rewriter/rewriters/default_optimizer.h"

namespace zorba {

#define ADD_DRIVER( d ) \
  m_childRewriters.push_back(rewriter_ptr_t (new d))
#define ADD_SINGLETON_DRIVER(rule) m_childRewriters.push_back(rewriter_ptr_t(new SingletonRuleMajorDriver<rule>))
#define ADD_SINGLETON_DRIVER_BASE(rule) \
  m_childRewriters.push_back(rewriter_ptr_t(new SingletonRuleMajorDriverBase (RuleMajorDriver::rule_ptr_t (new rule))))

#define ADD_RULE( rule ) \
  m_rules.push_back (rule_ptr_t (new rule))

  class FoldRules : public RuleMajorDriver {
  public:
    FoldRules() {
      ADD_RULE(MarkExpensiveOps);
      ADD_RULE(MarkUnfoldableExprs);
      ADD_RULE(MarkImpureExprs);
      // Most rules try to update the freevars annotations, but for now let's stay on the safe side
      ADD_RULE(MarkFreeVars);
      ADD_RULE(FoldConst (false));
      ADD_RULE(PartialEval);      
      ADD_RULE(RefactorPredFLWOR);
      ADD_RULE(EliminateUnusedLetVars);
    }
  };

DefaultOptimizer::DefaultOptimizer()
{
  ADD_SINGLETON_DRIVER(InferVarTypes);
  ADD_SINGLETON_DRIVER(EliminateTypeEnforcingOperations);

  ADD_SINGLETON_DRIVER(EliminateExtraneousPathSteps);

  ADD_DRIVER(FoldRules);

  // First eliminate operations which are not required UPSTREAM
  ADD_SINGLETON_DRIVER(MarkConsumerNodeProps);
  ADD_SINGLETON_DRIVER(EliminateProducerNodeOps);
  // Now eliminate operations which are redundant because of
  // properties of DOWNSTREAM expressions
  ADD_SINGLETON_DRIVER(MarkProducerNodeProps);
  ADD_SINGLETON_DRIVER(EliminateConsumerNodeOps);

  // For UDFs, which need this annotation in udf::requires_dyn_ctx()
  // TODO: only do this for UDFs
  ADD_SINGLETON_DRIVER(MarkUnfoldableExprs);
}

DefaultOptimizer::~DefaultOptimizer() throw ()
{
}

}
