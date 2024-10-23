# General code documentation rules #
Automatic code documentation shall be prepared during development process.
Documentation should cover:
- general solutions description of used mechanisms/patterns
- entity relationship model description
- detailed functional description
- implementation description.

Each file should be carefully checked before storing in the version control (whether
generated documentation meet the expectations, and there are no warnings related modified files):
@verbatim
$ cd build.*
$ make doc-generate
@endverbatim

## Documentation format ##
Documentation must be inlined in the code in the form of comments or put in separate files.

Inlined documentation must follow Doxygen formatting standard.
More detailed description should be in the form of markdown language (in .md files).

Both inlined and detailed might contain references to other objects (images, diagrams).

## Doxygen comments ##
Each entity in the code should have Doxygen comments.

General Qt notation must be used:
- `/*! .... */`
- no fences of `*`
- all commands start with `@`
- multiline comments are prefered
- `//` are allowed only in the case of post-descriptions (in the form `//!< ...`)
- `/*!` and `*/` in multiline comments should be kept alone
- one line comments are identical with @tc{brief} (and command might be ommited)

Comments shall match the indentation level of the code it documents.

Keep blocks simple. No ASCII arts allowed. Comments should contain meaningful information.
Each logical block shall be separated from next by empty line and appropriate @tc{header}.

Each part of code should be described. At least file, class, struct, enum, function level scope.

Do not state the obvious. It doesn't make a sense to describe empty destructor, default copy constructor, etc.

If Qt description style is used, empty line must follow @tc{brief} line, then follows description:
@verbatim
/*!
 * @brief Application model implementation
 *
 * This file contains all classes and functions, which are used by application model.
 * They define functional behaviour of each application.
 * @ref general_application_model
 *
@endverbatim

All sentences should end with `.` (dot). Brief description in JavaDoc style have to be separated from
description with`dot`.

One-line `/*! ... */` comments are allowed for very short descriptions. If brief description is put in the comment
and no extended description is present, @tc{brief} command must forerun this description.

Examples should be braced in @tc{verbatim} and @tc{endverbatim}. `*` in front of the line are not allowed.
Another form are files, they should be added by the means of @tc{include} directive.
File with example should be named according `example_xxx` pattern.

Don't use "black quotes" (`>` in front of the line). If list is intended proceed each entry with `-` (dash).

### File level description ###
Each file should start with comment block. The description goes first. It is not adviced to use JavaDoc
style here (@tc{brief} must be present).

After description block @tc{copyright} notice must be added:
@verbatim
 * @copyright Copyright (c) 2018-2019 Intel Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
@endverbatim

Another details (with links, references, etc) might be added afterwards.

Each file must have additional subsection:
@verbatim
 *
 * @file procedure_validator.hpp
 */
@endverbatim

### Class level description ###
Each class should be preceded with description block.
This block must contain at least description in Qt form. JavaDoc is not allowed.
It must be forerun with @tc{header} with appropriate title.

General concept of the top-level class should be put in front of the class (in description).
References to .md files, images, diagrams, etc are allowed (in the form of @tc{ref} or @tc{see}).

Class description should be kept with classes declaration.
Local classes and specializations should be documented in *.cpp files.

### Method/function level description ###
Each function should have description (including private/static functions).

Use @tc{param} to document all function parameters. For pointers and references
`[in]`, `[out]` or `[in, out]` should be added.
Don't use post description form here.

### Enum description ###
Describe enumeration with general description (@tc{brief} + description).
It is adviced to use post descriptions for all values:
@verbatim
/*!
 * @brief Value enumeration
 *
 * This enumeration describes values passed to...
 */
enum class EnumName : unsigned int {
    UNKNOWN, //!< not valid option
    VAL1, //!< used for option 1
    VALA, //!< used for option A
    VALB  //!< used for option B
};
@endverbatim

Pre-descriptive form with @tc{var} is allowed.

### Template level description ###
Describe templates as plain classes, methods, etc.

## References to other objects ##
It is suggested to embed external objects in the form:

@verbatim
 * ![embeded file title](embeded file name)
@endverbatim

File should be placed in the same directory where source file is located, or in `docs/images`.

## Additional commands ##
It is allowed to use @tc{todo} commands to indicate code, which should be improved or reworked.

Known issues must be described with @tc{bug} command.

Use @tc{deprecated} commands to indicate "old" code (not to be used), which should not be used.
The code must have attribute `deprecated` set, eg:

@verbatim
    /*! @deprecated To be used only from CommandJson. */
    explicit ProcedureValidator(const Procedure& procedure) __attribute__((deprecated));
@endverbatim