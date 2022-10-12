# VCU GUI Specific Contributing Guidelines

## Conventional Commits

Commits (and issue/pull request titles) should follow the [conventional commits specification](https://www.conventionalcommits.org/en/v1.0.0/) with the format:

```text
<type>(<scope>): <subject>
```

`<type>` **must** be one of:
| Type | Description |
|------------|---------------------------------------------------------------------|
| `feat` | New features |
| `fix` | Bug fixes |
| `refactor` | Refactors (rewriting/restructuring code without changing behaviour) |
| `perf` | Refactors specifically to improve performance |
| `style` | Code formatting, whitespace etc |
| `test` | New tests or changes to existing tests |
| `docs` | Changes to documentation |
| `build` | Changes to build system, compilation, toolchain |  
| `chore` | Chores (internal/organisational changes, updating .gitignore, etc) |
| `misc` | Anything which absolutely does not match any of the other types |

`<scope>` should be a single word describing a part of the system such as (but not limited to):
| Scope | Description |
|------------|----------------------------------------------------------------|
| gui | GUI components, windows, menu bar, appearance |
| config | VCU configuration model |
| comms | Communication with the VCU |
| settings | User settings |
| vscode | VS Code configuration |
| trunk | Trunk and linter/formatter configuration |

`<subject>` should written as "lowercase, imperative, present tense" (e.g. \_'create linear torque map interpolator').

## Branch Names

Branches should be named beginning with the `<type>` for the corresponding issue, followed by a `/` and a dash-separated title briefly describing the overall purpose of the branch:

```text
<type>/dash-separated-title
```

## Code Formatting and Linting

Code should be formatted and linted before committing using [Trunk](https://docs.trunk.io).
When you set up Trunk, allow it to automatically manage your git hooks for the repo to automate this process.
To check and format the code run:

```sh
trunk check
```

Install `trunk` for Linux or macOS with:

```sh
curl https://get.trunk.io -fsSL | bash
```

Alternatively, install the [VS Code extension](https://marketplace.visualstudio.com/items?itemName=Trunk.io).
