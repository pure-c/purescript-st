#include <purescript.h>

PURS_FFI_FUNC_3(Control_Monad_ST_Internal_map_, f, a, _, {
	return purs_any_app(f, purs_any_app(a, NULL));
});

PURS_FFI_FUNC_2(Control_Monad_ST_Internal_pure_, a, _, {
	return a;
});

PURS_FFI_FUNC_3(Control_Monad_ST_Internal_bind_, a, f, _, {
	return purs_any_app(purs_any_app(f, purs_any_app(a, NULL)), NULL);
});

PURS_FFI_FUNC_1(Control_Monad_ST_Internal_run, f, {
	return purs_any_app(f, NULL);
});

PURS_FFI_FUNC_3(Control_Monad_ST_Internal_while, f, a, _, {
	purs_any_while(purs_any_app(f, NULL)) {
		purs_any_app(a, NULL);
	}
	return NULL;
});

PURS_FFI_FUNC_4(Control_Monad_ST_Internal_for, _lo, _hi, f, _, {
	int lo = purs_any_get_int(_lo);
	int hi = purs_any_get_int(_hi);
	for (int i = 0; i < hi; i++) {
		purs_any_app(purs_any_app(f, purs_any_int_new(i)), NULL);
	}
	return NULL;
});

PURS_FFI_FUNC_3(Control_Monad_ST_Internal_foreach, _as, f, _, {
	const purs_vec_t * as = purs_any_get_array(_as);
	int i;
	const purs_any_t * tmp;
	purs_vec_foreach(as, tmp, i) {
		purs_any_app(purs_any_app(f, tmp), NULL);
	}
	return NULL;
});

PURS_FFI_FUNC_2(Control_Monad_ST_Internal_new, val, _, {
	return purs_any_foreign_new(NULL, (void *) val);
});

PURS_FFI_FUNC_2(Control_Monad_ST_Internal_read, _ref, _, {
	const purs_foreign_t * ref = purs_any_get_foreign(_ref);
	return ref->data;
});

PURS_FFI_FUNC_3(Control_Monad_ST_Internal_modify$, f, _ref, _, {
	purs_foreign_t * ref = (purs_foreign_t *) purs_any_get_foreign(_ref);
	const purs_any_t * _res = purs_any_app(f, ref->data);
	const purs_record_t * res = purs_any_get_record(_res);
	const purs_record_t * state = purs_record_find_by_key(res, "state");
	const purs_record_t * value = purs_record_find_by_key(res, "value");
	ref->data = (void *) state->value;
	return value->value;
});

PURS_FFI_FUNC_3(Control_Monad_ST_Internal_write, a, _ref, _, {
	purs_foreign_t * ref = (purs_foreign_t *) purs_any_get_foreign(_ref);
	ref->data = (void *) a;
	return NULL;
});
