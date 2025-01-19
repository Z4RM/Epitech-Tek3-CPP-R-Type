# Contributing

## Branch Naming Conventions

To ensure clarity and structure, follow these guidelines when naming branches:

### General Format

```
<type>/[optional-scope]/<short-description>
```

### Types of Branches

- **main**: The primary branch containing production-ready code.
- **dev**: The primary development branch containing pre-production code.
- A branch type can be either a _Conventional Commit_ type (cf. [Commit Message Conventions](#commit-message-conventions)) (except `feat` (replaced by `feature`), `chore` and `style` (both for minor changes, no need to do a branch)), or:
  - **`feature`**: For developing new features.
  - **`hotfix`**: For urgent fixes to production.

### Optional Scope

The optional scope is either `server` or `client`.
Whether it concerns both or something else, there is no need to precise it.

### Branch Naming Rules

- Use kebab-case: lowercase letters and hyphens (`-`) to separate words.
- Do not use special characters or spaces.
- Keep names concise but descriptive.

### Example

```
feature/server/client-authentification
```

## Commit Message Conventions

Follow the [Conventional Commits](https://conventionalcommits.org) format for clear and meaningful commit messages.

### General Format

```
<type>([optional-scope]): <description>

[optional body]

[optional footer(s)]
```

### Commit Types

- **feat**: A new feature.
  - Example: `feat(server-authentification): add client authentification`
- **fix**: A bugfix.
  - Example: `fix(client-ui): correct button alignment`
- **build**: Build changes.
  - Example: `build(server): add Asio library`
- **ci**: CI/CD changes.
  - Example: `ci(client): check code format in GitHub Actions workflow`
- **perf**: Performance improvements.
  - Example: `perf(server): reduce response time`
- **test**: Adding or updating tests.
  - Example: `test(server): add unit tests for payment processor`
- **docs**: Documentation changes.
  - Example: `docs(readme): update authors`
- **refactor**: Code refactoring without changing functionality.
  - Example: `refactor(server-authentification): separate code in multiple functions`
- **style**: Code style changes (formatting, re-indentation, etc.) with no functional effect.
  - Example: `style: reformat all files`
- **chore**: Miscellaneous tasks (e.g., editing the gitignore).
  - Example: `chore(gitignore): ignore CMake build directory`

### Commit Messages Rules

1. Write messages in the imperative mood (e.g., "add" instead of "added").
2. Separate the description from the body (and the body from the footer(s)) with an empty line.
3. Reference related issues or PRs (e.g., `Closes #123`) in footer(s).

## Pull Request Conventions/Rules

1. Avoid default PR name (generally the branch name with slashes replaced by spaces). The simplest is to name it following commit message conventions (without body or footer for sure).
2. Fill out the PR description with the Pull Request Template.
3. Request for people to review (add them to reviewers).
4. Wait for the PR to be reviewed.
5. Make changes if necessary.
6. Merge the PR (you can use the default commit name, or custom it, or use a _Conventional Commits_ one).
7. Delete the branch if it is no longer useful.
