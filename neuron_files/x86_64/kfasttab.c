/* Created by Language version: 6.2.0 */
/* NOT VECTORIZED */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "scoplib.h"
#undef PI
 
#include "md1redef.h"
#include "section.h"
#include "md2redef.h"

#if METHOD3
extern int _method3;
#endif

#undef exp
#define exp hoc_Exp
extern double hoc_Exp();
 
#define _threadargscomma_ /**/
#define _threadargs_ /**/
 	/*SUPPRESS 761*/
	/*SUPPRESS 762*/
	/*SUPPRESS 763*/
	/*SUPPRESS 765*/
	 extern double *getarg();
 static double *_p; static Datum *_ppvar;
 
#define t nrn_threads->_t
#define dt nrn_threads->_dt
#define gkbar _p[0]
#define ik _p[1]
#define n _p[2]
#define k _p[3]
#define ek _p[4]
#define Dn _p[5]
#define Dk _p[6]
#define _g _p[7]
#define _ion_ek	*_ppvar[0]._pval
#define _ion_ik	*_ppvar[1]._pval
#define _ion_dikdv	*_ppvar[2]._pval
 
#if MAC
#if !defined(v)
#define v _mlhv
#endif
#if !defined(h)
#define h _mlhh
#endif
#endif
 static int hoc_nrnpointerindex =  -1;
 /* external NEURON variables */
 /* declaration of user functions */
 static int _hoc_rates();
 static int _hoc_table_tabktau();
 static int _hoc_tabktau();
 static int _hoc_table_tabkinf();
 static int _hoc_tabkinf();
 static int _hoc_table_tabntau();
 static int _hoc_tabntau();
 static int _hoc_table_tabninf();
 static int _hoc_tabninf();
 static int _mechtype;
extern int nrn_get_mechtype();
 extern void _nrn_setdata_reg(int, void(*)(Prop*));
 static void _setdata(Prop* _prop) {
 _p = _prop->param; _ppvar = _prop->dparam;
 }
 static _hoc_setdata() {
 Prop *_prop, *hoc_getdata_range();
 _prop = hoc_getdata_range(_mechtype);
   _setdata(_prop);
 ret(1.);
}
 /* connect user functions to hoc names */
 static IntFunc hoc_intfunc[] = {
 "setdata_kfasttab", _hoc_setdata,
 "rates_kfasttab", _hoc_rates,
 "table_tabktau_kfasttab", _hoc_table_tabktau,
 "tabktau_kfasttab", _hoc_tabktau,
 "table_tabkinf_kfasttab", _hoc_table_tabkinf,
 "tabkinf_kfasttab", _hoc_tabkinf,
 "table_tabntau_kfasttab", _hoc_table_tabntau,
 "tabntau_kfasttab", _hoc_tabntau,
 "table_tabninf_kfasttab", _hoc_table_tabninf,
 "tabninf_kfasttab", _hoc_tabninf,
 0, 0
};
#define table_tabktau table_tabktau_kfasttab
#define tabktau tabktau_kfasttab
#define table_tabkinf table_tabkinf_kfasttab
#define tabkinf tabkinf_kfasttab
#define table_tabntau table_tabntau_kfasttab
#define tabntau tabntau_kfasttab
#define table_tabninf table_tabninf_kfasttab
#define tabninf tabninf_kfasttab
 extern double table_tabktau();
 extern double tabktau();
 extern double table_tabkinf();
 extern double tabkinf();
 extern double table_tabntau();
 extern double tabntau();
 extern double table_tabninf();
 extern double tabninf();
 /* declare global and static user variables */
#define ktau ktau_kfasttab
 double ktau = 0;
#define kinf kinf_kfasttab
 double kinf = 0;
#define ntau ntau_kfasttab
 double ntau = 0;
#define ninf ninf_kfasttab
 double ninf = 0;
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 "gkbar_kfasttab", 0, 1e+09,
 0,0,0
};
 static HocParmUnits _hoc_parm_units[] = {
 "ntau_kfasttab", "ms",
 "ktau_kfasttab", "ms",
 "gkbar_kfasttab", "mho/cm2",
 "ik_kfasttab", "mA/cm2",
 0,0
};
 static double delta_t = 1;
 static double k0 = 0;
 static double n0 = 0;
 static double v = 0;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 "ninf_kfasttab", &ninf_kfasttab,
 "kinf_kfasttab", &kinf_kfasttab,
 "ntau_kfasttab", &ntau_kfasttab,
 "ktau_kfasttab", &ktau_kfasttab,
 0,0
};
 static DoubVec hoc_vdoub[] = {
 0,0,0
};
 static double _sav_indep;
 static void nrn_alloc(), nrn_init(), nrn_state();
 static void nrn_cur(), nrn_jacob();
 
static int _ode_count(), _ode_map(), _ode_spec(), _ode_matsol();
 
#define _cvode_ieq _ppvar[3]._i
 /* connect range variables in _p that hoc is supposed to know about */
 static char *_mechanism[] = {
 "6.2.0",
"kfasttab",
 "gkbar_kfasttab",
 0,
 "ik_kfasttab",
 0,
 "n_kfasttab",
 "k_kfasttab",
 0,
 0};
 static Symbol* _k_sym;
 
static void nrn_alloc(_prop)
	Prop *_prop;
{
	Prop *prop_ion, *need_memb();
	double *_p; Datum *_ppvar;
 	_p = nrn_prop_data_alloc(_mechtype, 8, _prop);
 	/*initialize range parameters*/
 	gkbar = 0.12;
 	_prop->param = _p;
 	_prop->param_size = 8;
 	_ppvar = nrn_prop_datum_alloc(_mechtype, 4, _prop);
 	_prop->dparam = _ppvar;
 	/*connect ionic variables to this model*/
 prop_ion = need_memb(_k_sym);
 nrn_promote(prop_ion, 0, 1);
 	_ppvar[0]._pval = &prop_ion->param[0]; /* ek */
 	_ppvar[1]._pval = &prop_ion->param[3]; /* ik */
 	_ppvar[2]._pval = &prop_ion->param[4]; /* _ion_dikdv */
 
}
 static _initlists();
  /* some states have an absolute tolerance */
 static Symbol** _atollist;
 static HocStateTolerance _hoc_state_tol[] = {
 0,0
};
 static void _update_ion_pointer(Datum*);
 _kfasttab_reg() {
	int _vectorized = 0;
  _initlists();
 	ion_reg("k", -10000.);
 	_k_sym = hoc_lookup("k_ion");
 	register_mech(_mechanism, nrn_alloc,nrn_cur, nrn_jacob, nrn_state, nrn_init, hoc_nrnpointerindex, 0);
 _mechtype = nrn_get_mechtype(_mechanism[1]);
     _nrn_setdata_reg(_mechtype, _setdata);
     _nrn_thread_reg(_mechtype, 2, _update_ion_pointer);
  hoc_register_dparam_size(_mechtype, 4);
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 	hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 kfasttab /home/bernhard/workspace/olfactory_system/OlfactorySystem/neuron_files/x86_64/kfasttab.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
static int _reset;
static char *modelname = "HH fast potassium channel with FUNCTION_TABLEs";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static _modl_cleanup(){ _match_recurse=1;}
static rates();
 
static int _ode_spec1(), _ode_matsol1();
 
static void* _ptable_tabktau = (void*)0;
 
static void* _ptable_tabkinf = (void*)0;
 
static void* _ptable_tabntau = (void*)0;
 
static void* _ptable_tabninf = (void*)0;
 
extern double hoc_func_table();
 static int _slist1[2], _dlist1[2];
 static int states();
 
/*CVODE*/
 static int _ode_spec1 () {_reset=0;
 {
   rates ( _threadargscomma_ v ) ;
   Dn = ( ninf - n ) / ntau ;
   Dk = ( kinf - k ) / ktau ;
   }
 return _reset;
}
 static int _ode_matsol1 () {
 rates ( _threadargscomma_ v ) ;
 Dn = Dn  / (1. - dt*( ( ( ( - 1.0 ) ) ) / ntau )) ;
 Dk = Dk  / (1. - dt*( ( ( ( - 1.0 ) ) ) / ktau )) ;
}
 /*END CVODE*/
 static int states () {_reset=0;
 {
   rates ( _threadargscomma_ v ) ;
    n = n + (1. - exp(dt*(( ( ( - 1.0 ) ) ) / ntau)))*(- ( ( ( ninf ) ) / ntau ) / ( ( ( ( - 1.0) ) ) / ntau ) - n) ;
    k = k + (1. - exp(dt*(( ( ( - 1.0 ) ) ) / ktau)))*(- ( ( ( kinf ) ) / ktau ) / ( ( ( ( - 1.0) ) ) / ktau ) - k) ;
   }
  return 0;
}
 
double tabninf (  _lv )  
	double _lv ;
 {
 double _arg[1];
 _arg[0] = _lv;
 return hoc_func_table(_ptable_tabninf, 1, _arg);
 }
/*  }
  */
 
static int _hoc_tabninf() {
  double _r;
   _r =  tabninf (  *getarg(1) ) ;
 ret(_r);
}
 double table_tabninf ( ) {
	hoc_spec_table(&_ptable_tabninf, 1);
	return 0.;
}
 
static int _hoc_table_tabninf() {
  double _r;
   _r =  table_tabninf ( ) ;
 ret(_r);
}
 
double tabntau (  _lv )  
	double _lv ;
 {
 double _arg[1];
 _arg[0] = _lv;
 return hoc_func_table(_ptable_tabntau, 1, _arg);
 }
/*  }
  */
 
static int _hoc_tabntau() {
  double _r;
   _r =  tabntau (  *getarg(1) ) ;
 ret(_r);
}
 double table_tabntau ( ) {
	hoc_spec_table(&_ptable_tabntau, 1);
	return 0.;
}
 
static int _hoc_table_tabntau() {
  double _r;
   _r =  table_tabntau ( ) ;
 ret(_r);
}
 
double tabkinf (  _lv )  
	double _lv ;
 {
 double _arg[1];
 _arg[0] = _lv;
 return hoc_func_table(_ptable_tabkinf, 1, _arg);
 }
/*  }
  */
 
static int _hoc_tabkinf() {
  double _r;
   _r =  tabkinf (  *getarg(1) ) ;
 ret(_r);
}
 double table_tabkinf ( ) {
	hoc_spec_table(&_ptable_tabkinf, 1);
	return 0.;
}
 
static int _hoc_table_tabkinf() {
  double _r;
   _r =  table_tabkinf ( ) ;
 ret(_r);
}
 
double tabktau (  _lv )  
	double _lv ;
 {
 double _arg[1];
 _arg[0] = _lv;
 return hoc_func_table(_ptable_tabktau, 1, _arg);
 }
/*  }
  */
 
static int _hoc_tabktau() {
  double _r;
   _r =  tabktau (  *getarg(1) ) ;
 ret(_r);
}
 double table_tabktau ( ) {
	hoc_spec_table(&_ptable_tabktau, 1);
	return 0.;
}
 
static int _hoc_table_tabktau() {
  double _r;
   _r =  table_tabktau ( ) ;
 ret(_r);
}
 
static int  rates (  _lv )  
	double _lv ;
 {
   ninf = tabninf ( _threadargscomma_ _lv ) ;
   ntau = tabntau ( _threadargscomma_ _lv ) ;
   kinf = tabkinf ( _threadargscomma_ _lv ) ;
   ktau = tabktau ( _threadargscomma_ _lv ) ;
    return 0; }
 
static int _hoc_rates() {
  double _r;
   _r = 1.;
 rates (  *getarg(1) ) ;
 ret(_r);
}
 
static int _ode_count(_type) int _type;{ return 2;}
 
static int _ode_spec(_NrnThread* _nt, _Memb_list* _ml, int _type) {
   Datum* _thread;
   Node* _nd; double _v; int _iml, _cntml;
  _cntml = _ml->_nodecount;
  _thread = _ml->_thread;
  for (_iml = 0; _iml < _cntml; ++_iml) {
    _p = _ml->_data[_iml]; _ppvar = _ml->_pdata[_iml];
    _nd = _ml->_nodelist[_iml];
    v = NODEV(_nd);
  ek = _ion_ek;
     _ode_spec1 ();
  }}
 
static int _ode_map(_ieq, _pv, _pvdot, _pp, _ppd, _atol, _type) int _ieq, _type; double** _pv, **_pvdot, *_pp, *_atol; Datum* _ppd; { 
 	int _i; _p = _pp; _ppvar = _ppd;
	_cvode_ieq = _ieq;
	for (_i=0; _i < 2; ++_i) {
		_pv[_i] = _pp + _slist1[_i];  _pvdot[_i] = _pp + _dlist1[_i];
		_cvode_abstol(_atollist, _atol, _i);
	}
 }
 
static int _ode_matsol(_NrnThread* _nt, _Memb_list* _ml, int _type) {
   Datum* _thread;
   Node* _nd; double _v; int _iml, _cntml;
  _cntml = _ml->_nodecount;
  _thread = _ml->_thread;
  for (_iml = 0; _iml < _cntml; ++_iml) {
    _p = _ml->_data[_iml]; _ppvar = _ml->_pdata[_iml];
    _nd = _ml->_nodelist[_iml];
    v = NODEV(_nd);
  ek = _ion_ek;
 _ode_matsol1 ();
 }}
 extern void nrn_update_ion_pointer(Symbol*, Datum*, int, int);
 static void _update_ion_pointer(Datum* _ppvar) {
   nrn_update_ion_pointer(_k_sym, _ppvar, 0, 0);
   nrn_update_ion_pointer(_k_sym, _ppvar, 1, 3);
   nrn_update_ion_pointer(_k_sym, _ppvar, 2, 4);
 }

static void initmodel() {
  int _i; double _save;_ninits++;
 _save = t;
 t = 0.0;
{
  k = k0;
  n = n0;
 {
   rates ( _threadargscomma_ v ) ;
   n = ninf ;
   k = kinf ;
   }
  _sav_indep = t; t = _save;

}
}

static void nrn_init(_NrnThread* _nt, _Memb_list* _ml, int _type){
Node *_nd; double _v; int* _ni; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
for (_iml = 0; _iml < _cntml; ++_iml) {
 _p = _ml->_data[_iml]; _ppvar = _ml->_pdata[_iml];
#if CACHEVEC
  if (use_cachevec) {
    _v = VEC_V(_ni[_iml]);
  }else
#endif
  {
    _nd = _ml->_nodelist[_iml];
    _v = NODEV(_nd);
  }
 v = _v;
  ek = _ion_ek;
 initmodel();
 }}

static double _nrn_current(double _v){double _current=0.;v=_v;{ {
   ik = gkbar * n * n * k * ( v - ek ) ;
   }
 _current += ik;

} return _current;
}

static void nrn_cur(_NrnThread* _nt, _Memb_list* _ml, int _type){
Node *_nd; int* _ni; double _rhs, _v; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
for (_iml = 0; _iml < _cntml; ++_iml) {
 _p = _ml->_data[_iml]; _ppvar = _ml->_pdata[_iml];
#if CACHEVEC
  if (use_cachevec) {
    _v = VEC_V(_ni[_iml]);
  }else
#endif
  {
    _nd = _ml->_nodelist[_iml];
    _v = NODEV(_nd);
  }
  ek = _ion_ek;
 _g = _nrn_current(_v + .001);
 	{ double _dik;
  _dik = ik;
 _rhs = _nrn_current(_v);
  _ion_dikdv += (_dik - ik)/.001 ;
 	}
 _g = (_g - _rhs)/.001;
  _ion_ik += ik ;
#if CACHEVEC
  if (use_cachevec) {
	VEC_RHS(_ni[_iml]) -= _rhs;
  }else
#endif
  {
	NODERHS(_nd) -= _rhs;
  }
 
}}

static void nrn_jacob(_NrnThread* _nt, _Memb_list* _ml, int _type){
Node *_nd; int* _ni; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
for (_iml = 0; _iml < _cntml; ++_iml) {
 _p = _ml->_data[_iml];
#if CACHEVEC
  if (use_cachevec) {
	VEC_D(_ni[_iml]) += _g;
  }else
#endif
  {
     _nd = _ml->_nodelist[_iml];
	NODED(_nd) += _g;
  }
 
}}

static void nrn_state(_NrnThread* _nt, _Memb_list* _ml, int _type){
 double _break, _save;
Node *_nd; double _v; int* _ni; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
for (_iml = 0; _iml < _cntml; ++_iml) {
 _p = _ml->_data[_iml]; _ppvar = _ml->_pdata[_iml];
 _nd = _ml->_nodelist[_iml];
#if CACHEVEC
  if (use_cachevec) {
    _v = VEC_V(_ni[_iml]);
  }else
#endif
  {
    _nd = _ml->_nodelist[_iml];
    _v = NODEV(_nd);
  }
 _break = t + .5*dt; _save = t;
 v=_v;
{
  ek = _ion_ek;
 { {
 for (; t < _break; t += dt) {
 error =  states();
 if(error){fprintf(stderr,"at line 44 in file kfasttab.mod:\n	SOLVE states METHOD cnexp\n"); nrn_complain(_p); abort_run(error);}
 
}}
 t = _save;
 } }}

}

static terminal(){}

static _initlists() {
 int _i; static int _first = 1;
  if (!_first) return;
 _slist1[0] = &(n) - _p;  _dlist1[0] = &(Dn) - _p;
 _slist1[1] = &(k) - _p;  _dlist1[1] = &(Dk) - _p;
_first = 0;
}