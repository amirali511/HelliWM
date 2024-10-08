#!/bin/python3

contents = []
word = ""
line = []


# Opening the input file and reading the contents
with open(".shortcut", 'r') as file:
    chars = file.read()
    for char in chars:
        if char == " ":
            line.append(word)
            word = ""
        elif char == "\n":
            line.append(word)
            contents.append(line)
            line = []
            word = ""
        else:
            word += char

    if word:
        line.append(word)
    if line:
        contents.append(line)

# Writing it out
with open("shortcut.h", "w") as output:
    output.write("#define HSETUP()\\\n")
    for i in contents[0][0:-1]:
        output.write(f"  xcb_keycode_t {i}_keycode = xcb_key_symbols_get_keycode (keysyms, {contents[1][contents[0].index(i)]})[0];\\\n")
        output.write(f"  cookie = xcb_grab_key (connection, 1, screen->root, MOD, {i}_keycode, XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);\\\n")
        output.write(f'  if ({i}_keycode == NULL) PANIC ("The {i} keycode is NULL, aborting...\\n", {i}_keycode == 0);\\\n')
        output.write(f'	 if (xcb_request_check (connection, cookie)) PANIC ("Could not grab the {i} key, aborting...\\n", xcb_request_check (connection, cookie));\\\n')
    
    output.write(f"  xcb_keycode_t {contents[0][-1]}_keycode = xcb_key_symbols_get_keycode (keysyms, {contents[1][-1]})[0];\\\n")
    output.write(f"  xcb_grab_key (connection, 1, screen->root, MOD, {contents[0][-1]}_keycode, XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);\\\n")
    output.write(f'  if ({contents[0][-1]}_keycode == NULL) PANIC ("The {contents[0][-1]} keycode is NULL, aborting...\\n", {contents[0][-1]}_keycode == 0);\\\n')
    output.write(f'	 if (xcb_request_check (connection, cookie)) PANIC ("Could not grab the {contents[0][-1]} key, aborting...\\n", xcb_request_check (connection, cookie));\n')

    output.write("\n\n")
    output.write("#define HHANDLE()\\\n")
    for j in contents[0][0:-1]:
        output.write(f"  else if ((key_press_event->state & MOD) && (key_press_event->detail == {j}_keycode)) {{ run (\"{j}\"); }}\\\n")
    output.write(f"  else if ((key_press_event->state & MOD) && (key_press_event->detail == {contents[0][-1]}_keycode)) {{ run (\"{contents[0][-1]}\"); }}\n")
    output.write("\n")

