/* rpn_list_get_name() {{{ */

/******************************************************************************

					RPN_LIST_GET_NAME

******************************************************************************/
char *rpn_list_get_name(rpn_elt *elt_list)
{
	int						 _type;

	if ((_type = rpn_get_type(elt_list)) != RPN_TYPE_LIST) {
		fprintf(stderr, "%s: %s() : bad element type (%s) !\n",
		        G.progname, __func__, rpn_type_to_string(_type);
			   exit(RPN_EXIT_
	}

}

/* rpn_list_get_name() }}} */


