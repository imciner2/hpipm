/**************************************************************************************************
*                                                                                                 *
* This file is part of HPIPM.                                                                     *
*                                                                                                 *
* HPIPM -- High-Performance Interior Point Method.                                                *
* Copyright (C) 2019 by Gianluca Frison.                                                          *
* Developed at IMTEK (University of Freiburg) under the supervision of Moritz Diehl.              *
* All rights reserved.                                                                            *
*                                                                                                 *
* The 2-Clause BSD License                                                                        *
*                                                                                                 *
* Redistribution and use in source and binary forms, with or without                              *
* modification, are permitted provided that the following conditions are met:                     *
*                                                                                                 *
* 1. Redistributions of source code must retain the above copyright notice, this                  *
*    list of conditions and the following disclaimer.                                             *
* 2. Redistributions in binary form must reproduce the above copyright notice,                    *
*    this list of conditions and the following disclaimer in the documentation                    *
*    and/or other materials provided with the distribution.                                       *
*                                                                                                 *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND                 *
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED                   *
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE                          *
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR                 *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES                  *
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;                    *
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND                     *
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT                      *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS                   *
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                    *
*                                                                                                 *
* Author: Gianluca Frison, gianluca.frison (at) imtek.uni-freiburg.de                             *
*                                                                                                 *
**************************************************************************************************/



#ifndef HPIPM_D_TREE_OCP_QP_IPM_H_
#define HPIPM_D_TREE_OCP_QP_IPM_H_



#include <blasfeo_target.h>
#include <blasfeo_common.h>

#include <hpipm_common.h>
#include <hpipm_d_tree_ocp_qp_dim.h>
#include <hpipm_d_tree_ocp_qp.h>
#include <hpipm_d_tree_ocp_qp_res.h>
#include <hpipm_d_tree_ocp_qp_sol.h>



#ifdef __cplusplus
extern "C" {
#endif



struct d_tree_ocp_qp_ipm_arg
	{
	double mu0; // initial value for duality measure
	double alpha_min; // exit cond on step length
	double res_g_max; // exit cond on inf norm of residuals
	double res_b_max; // exit cond on inf norm of residuals
	double res_d_max; // exit cond on inf norm of residuals
	double res_m_max; // exit cond on inf norm of residuals
	double reg_prim; // reg of primal hessian
	double lam_min; // min value in lam vector
	double t_min; // min value in t vector
	int iter_max; // exit cond in iter number
	int stat_max; // iterations saved in stat
	int pred_corr; // use Mehrotra's predictor-corrector IPM algirthm
	int cond_pred_corr; // conditional Mehrotra's predictor-corrector
	int itref_pred_max; // max number of iterative refinement steps for predictor step
	int itref_corr_max; // max number of iterative refinement steps for corrector step
	int warm_start; // 0 no warm start, 1 warm start primal sol, 2 warm start primal and dual sol
	int lq_fact; // 0 syrk+potrf, 1 mix, 2 lq
	int abs_form; // absolute IPM formulation
	int comp_dual_sol; // dual solution (only for abs_form==1)
	int comp_res_exit; // compute residuals on exit (only for abs_form==1 and comp_dual_sol==1)
	int memsize;
	};



struct d_tree_ocp_qp_ipm_workspace
	{
	struct d_core_qp_ipm_workspace *core_workspace;
	struct d_tree_ocp_qp_res_workspace *res_workspace;
	struct d_tree_ocp_qp_sol *sol_step;
	struct d_tree_ocp_qp_sol *sol_itref;
	struct d_tree_ocp_qp *qp_step;
	struct d_tree_ocp_qp *qp_itref;
	struct d_tree_ocp_qp_res *res_itref;
	struct d_tree_ocp_qp_res *res;
	struct blasfeo_dvec *Gamma; // hessian update
	struct blasfeo_dvec *gamma; // hessian update
	struct blasfeo_dvec *tmp_nxM; // work space of size nxM
	struct blasfeo_dvec *tmp_nbgM; // work space of size nbgM
	struct blasfeo_dvec *tmp_nsM; // work space of size nsM
	struct blasfeo_dvec *Pb; // Pb
	struct blasfeo_dvec *Zs_inv;
	struct blasfeo_dmat *L;
	struct blasfeo_dmat *Lh;
	struct blasfeo_dmat *AL;
	struct blasfeo_dmat *lq0;
	struct blasfeo_dvec *tmp_m;
	double *stat; // convergence statistics
	int *use_hess_fact;
	void *lq_work0;
	double qp_res[4]; // infinity norm of residuals
	int iter; // iteration number
	int stat_max; // iterations saved in stat
	int use_Pb;
	int memsize;
	};



//
int d_memsize_tree_ocp_qp_ipm_arg(struct d_tree_ocp_qp_dim *dim);
//
void d_create_tree_ocp_qp_ipm_arg(struct d_tree_ocp_qp_dim *dim, struct d_tree_ocp_qp_ipm_arg *arg, void *mem);
//
void d_set_default_tree_ocp_qp_ipm_arg(enum hpipm_mode mode, struct d_tree_ocp_qp_ipm_arg *arg);
//
void d_set_tree_ocp_qp_ipm_arg_iter_max(int iter_max, struct d_tree_ocp_qp_ipm_arg *arg);
//
void d_set_tree_ocp_qp_ipm_arg_mu0(double mu0, struct d_tree_ocp_qp_ipm_arg *arg);
//
void d_set_tree_ocp_qp_ipm_arg_tol_stat(double tol_stat, struct d_tree_ocp_qp_ipm_arg *arg);
//
void d_set_tree_ocp_qp_ipm_arg_tol_eq(double tol_eq, struct d_tree_ocp_qp_ipm_arg *arg);
//
void d_set_tree_ocp_qp_ipm_arg_tol_ineq(double tol_ineq, struct d_tree_ocp_qp_ipm_arg *arg);
//
void d_set_tree_ocp_qp_ipm_arg_tol_comp(double tol_comp, struct d_tree_ocp_qp_ipm_arg *arg);
//
void d_set_tree_ocp_qp_ipm_arg_reg_prim(double reg, struct d_tree_ocp_qp_ipm_arg *arg);

//
int d_memsize_tree_ocp_qp_ipm(struct d_tree_ocp_qp_dim *dim, struct d_tree_ocp_qp_ipm_arg *arg);
//
void d_create_tree_ocp_qp_ipm(struct d_tree_ocp_qp_dim *dim, struct d_tree_ocp_qp_ipm_arg *arg, struct d_tree_ocp_qp_ipm_workspace *ws, void *mem);
//
int d_get_tree_ocp_qp_ipm_iter(struct d_tree_ocp_qp_ipm_workspace *ws);
//
double d_get_tree_ocp_qp_ipm_res_stat(struct d_tree_ocp_qp_ipm_workspace *ws);
//
double d_get_tree_ocp_qp_ipm_res_eq(struct d_tree_ocp_qp_ipm_workspace *ws);
//
double d_get_tree_ocp_qp_ipm_res_ineq(struct d_tree_ocp_qp_ipm_workspace *ws);
//
double d_get_tree_ocp_qp_ipm_res_comp(struct d_tree_ocp_qp_ipm_workspace *ws);
//
double *d_get_tree_ocp_qp_ipm_stat(struct d_tree_ocp_qp_ipm_workspace *ws);
//
int d_solve_tree_ocp_qp_ipm(struct d_tree_ocp_qp *qp, struct d_tree_ocp_qp_sol *qp_sol, struct d_tree_ocp_qp_ipm_arg *arg, struct d_tree_ocp_qp_ipm_workspace *ws);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif // HPIPM_D_TREE_OCP_QP_IPM_H_
