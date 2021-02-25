#include <purescript.h>

PURS_FFI_FUNC_3(Control_Monad_ST_Internal_map_, f, a, _) {
	purs_any_t v0 = purs_any_app(a, purs_any_null);
	purs_any_t result = purs_any_app(f, v0);
	PURS_ANY_RELEASE(v0);
	return result;
}

PURS_FFI_FUNC_2(Control_Monad_ST_Internal_pure_, a, _) {
	PURS_ANY_RETAIN(a);
	return a;
}

PURS_FFI_FUNC_3(Control_Monad_ST_Internal_bind_, a, f, _) {
	purs_any_t v0 = purs_any_app(a, purs_any_null);
	purs_any_t v1 = purs_any_app(f, v0);
	purs_any_t result = purs_any_app(v1, purs_any_null);
	PURS_ANY_RELEASE(v1);
	PURS_ANY_RELEASE(v0);
	return result;
}

PURS_FFI_FUNC_1(Control_Monad_ST_Internal_run, f) {
	return purs_any_app(f, purs_any_null);
}

PURS_FFI_FUNC_3(Control_Monad_ST_Internal_while, f, a, _) {
	while (purs_any_is_true(purs_any_app(f, purs_any_null))) {
		PURS_ANY_RELEASE(purs_any_app(a, purs_any_null));
	}
	return purs_any_null;
}

PURS_FFI_FUNC_4(Control_Monad_ST_Internal_for, _lo, _hi, f, _) {
	int lo = purs_any_force_int(_lo);
	int hi = purs_any_force_int(_hi);
	for (int i = lo; i < hi; i++) {
		purs_any_t v0 = purs_any_app(f, purs_any_int(i));
		purs_any_t v1 = purs_any_app(v0, purs_any_null);
		PURS_ANY_RELEASE(v1);
		PURS_ANY_RELEASE(v0);
	}
	return purs_any_null;
}

PURS_FFI_FUNC_3(Control_Monad_ST_Internal_foreach, _as, f, _) {
	const purs_vec_t *as = purs_any_force_array(_as);
	int i;
	purs_any_t tmp;
	purs_vec_foreach(as, tmp, i) {
		purs_any_t v0 = purs_any_app(f, tmp);
		purs_any_t v1 = purs_any_app(v0, purs_any_null);
		PURS_ANY_RELEASE(v1);
		PURS_ANY_RELEASE(v0);
	}
	PURS_RC_RELEASE(as);
	return purs_any_null;
}

static void Control_Monad_ST_Internal_finalize(void *tag, void *data) {
	purs_any_t any = *((const purs_any_t*) data);
	PURS_ANY_RELEASE(any);
	purs_free(data);
}

PURS_FFI_FUNC_2(Control_Monad_ST_Internal_new, val, _) {
	/* retain value: the st keeps it alive */
	PURS_ANY_RETAIN(val);

	/* move value onto heap */
	purs_any_t *valheap = purs_new(purs_any_t);
	memcpy(valheap, &val, sizeof(purs_any_t));

	const purs_foreign_t *foreign = purs_foreign_new(
	    NULL,
	    (void *) valheap,
	    Control_Monad_ST_Internal_finalize);
	return purs_any_foreign(foreign);
}

PURS_FFI_FUNC_2(Control_Monad_ST_Internal_read, _ref, _) {
	const purs_foreign_t *foreign = purs_any_unsafe_get_foreign(_ref);
	purs_any_t any = *((const purs_any_t*) foreign->data);
	return any;
}

static purs_str_t state_str = purs_str_static_lazy("state");
static purs_str_t value_str = purs_str_static_lazy("value");

PURS_FFI_FUNC_3(Control_Monad_ST_Internal_modify$, f, _ref, _) {
	const purs_foreign_t *foreign = purs_any_unsafe_get_foreign(_ref);
	purs_any_t currentstate = *((const purs_any_t*) foreign->data);

	const purs_any_t v0 = purs_any_app(f, currentstate);
	const purs_record_t *res = purs_any_force_record(v0);

	purs_any_t nextstate = *(purs_record_find_by_key(res, &state_str));
	purs_any_t retvalue = *(purs_record_find_by_key(res, &value_str));

	PURS_ANY_RELEASE(currentstate);
	PURS_ANY_RETAIN(nextstate);

	/* move state onto heap */
	memcpy(foreign->data, &nextstate, sizeof(purs_any_t));

	PURS_RC_RELEASE(res);
	PURS_ANY_RELEASE(v0);

	return retvalue;
}

PURS_FFI_FUNC_3(Control_Monad_ST_Internal_write, nextstate, _ref, _) {
	const purs_foreign_t *foreign = purs_any_unsafe_get_foreign(_ref);
	purs_any_t currentstate = *((const purs_any_t*) foreign->data);

	PURS_ANY_RELEASE(currentstate);
	PURS_ANY_RETAIN(nextstate);

	/* move state onto heap */
	memcpy(foreign->data, &nextstate, sizeof(purs_any_t));

	return purs_any_null;
}
