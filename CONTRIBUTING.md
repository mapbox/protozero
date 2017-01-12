# Contributing to protozero

## Releasing

To release a new protozero version:

 - Make sure all tests are passing locally, on travis and on appveyor
 - Make sure "make doc" builds
 - Update version number in
   - include/protozero/version.hpp (two places)
   - package.json
 - Update CHANGELOG.md
   (don't forget links at the bottom of the file)
 - Update UPGRADING.md if necessary
 - `git commit -m "Release X.Y.Z" include/protozero/version.hpp package.json CHANGELOG.md UPGRADING.md`
 - `git tag vX.Y.Z`
 - `git push`
 - `git push --tags`
 - Go to https://github.com/mapbox/protozero/releases
   and edit the new release. Put "Version x.y.z" in title and
   cut-and-paste entry from CHANGELOG.md.
 - Publish to npm:
   - First run `make testpack` to see what files will be published
   - If you see any unwanted files, then add them to `.npmignore`
   - Then run: `npm publish`

