import { sayHi } from '../js/tt.js';

// Run the test
test('Returns a greeting as a string', function () {

    expect(typeof sayHi()).toBe('string');
    // should include the provided name
    expect(sayHi('Merlin').includes('Merlin')).toBe(true);
});