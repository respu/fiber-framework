#include "private/ff_common.h"

#include "private/ff_stack.h"

struct stack_entry
{
	struct stack_entry *next;
	const void *data;
};

struct ff_stack
{
	struct stack_entry *top;
};

struct ff_stack *ff_stack_create()
{
	struct ff_stack *stack;

	stack = (struct ff_stack *) ff_malloc(sizeof(*stack));
	stack->top = NULL;

	return stack;
}

void ff_stack_delete(struct ff_stack *stack)
{
	ff_assert(stack->top == NULL);

	ff_free(stack);
}

void ff_stack_push(struct ff_stack *stack, const void *data)
{
	struct stack_entry *entry;

	entry = (struct stack_entry *) ff_malloc(sizeof(*entry));
	entry->next = stack->top;
	entry->data = data;
	stack->top = entry;
}

int ff_stack_is_empty(struct ff_stack *stack)
{
	int is_empty;

	is_empty = stack->top == NULL ? 1 : 0;
	return is_empty;
}

void ff_stack_top(struct ff_stack *stack, const void **data)
{
	ff_assert(stack->top != NULL);

	*data = stack->top->data;
}

void ff_stack_pop(struct ff_stack *stack)
{
	struct stack_entry *entry;

	ff_assert(stack->top != NULL);

	entry = stack->top;
	stack->top = entry->next;
	ff_free(entry);
}

enum ff_result ff_stack_remove_entry(struct ff_stack *stack, const void *data)
{
	struct stack_entry **entry_ptr;
	struct stack_entry *entry;
	enum ff_result result = FF_FAILURE;

	entry_ptr = &stack->top;
	entry = stack->top;
	while (entry != NULL)
	{
		if (entry->data == data)
		{
			*entry_ptr = entry->next;
			ff_free(entry);
			result = FF_SUCCESS;
			break;
		}
		entry_ptr = &entry->next;
		entry = entry->next;
	}
	if (result != FF_SUCCESS)
	{
		ff_log_debug(L"the stack=%p doesn't contain the given data=%p", stack, data);
	}

	return result;
}
