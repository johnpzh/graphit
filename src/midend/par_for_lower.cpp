//
// Created by Tugsbayasgalan Manlaibaatar on 2020-04-08.
//

#include <graphit/midend/par_for_lower.h>

namespace graphit {

    void ParForLower::lower() {
        auto lower_par_for_stmt = LowerParForStmt(schedule_, mir_context_);
        std::vector<mir::FuncDecl::Ptr> functions = mir_context_->getFunctionList();
        for (auto function : functions) {
            lower_par_for_stmt.rewrite(function);
        }

    }

    void ParForLower::LowerParForStmt::visit(mir::ParForStmt::Ptr par_for) {

        par_for->grain_size = 0;
        if (schedule_ != nullptr && schedule_->par_for_grain_size_schedules != nullptr) {
            //TODO: why is there no current scope?
            //auto current_scope_name = label_scope_.getCurrentScope();
            auto par_for_schedule = schedule_->par_for_grain_size_schedules->find(par_for->stmt_label);
            if (par_for_schedule != schedule_->par_for_grain_size_schedules->end()) {
                //if a schedule for the statement has been found
                par_for->grain_size = par_for_schedule->second;
            }
        }

        par_for->type = ParForSchedule::ParForType::STATIC;

        if (schedule_ != nullptr && schedule_->par_for_type_schedules != nullptr) {
            //TODO: why is there no current scope?
            //auto current_scope_name = label_scope_.getCurrentScope();
            auto par_for_schedule = schedule_->par_for_type_schedules->find(par_for->stmt_label);
            if (par_for_schedule != schedule_->par_for_type_schedules->end()) {
                //if a schedule for the statement has been found
                par_for->type = par_for_schedule->second;
            }
        }


        if (schedule_ != nullptr && schedule_->par_for_num_threads != nullptr) {
            //TODO: why is there no current scope?
            //auto current_scope_name = label_scope_.getCurrentScope();
            auto par_for_schedule = schedule_->par_for_num_threads->find(par_for->stmt_label);
            if (par_for_schedule != schedule_->par_for_num_threads->end()) {
                //if a schedule for the statement has been found
                par_for->num_threads = par_for_schedule->second;
            }
        }
        node = par_for;

    }

}