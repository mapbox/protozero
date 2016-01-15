# Contributing to protozero

## Releasing

To release a new protozero version:

 - Make sure all tests are passing on travis and appveyor
 - Create a new tag and push to github `git push --tags`
 - Publish to npm:
   - First run `make testpack` to see what files will be published
   - If you see any unwanted files, then add them to `.npmignore`
   - Then run: `npm publish`