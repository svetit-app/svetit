import { TestBed, inject, waitForAsync } from '@angular/core/testing';

import { SchemeLoadGuard } from './scheme-load.guard';

describe('SchemeLoadGuard', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [SchemeLoadGuard]
    });
  });

  it('should ...', inject([SchemeLoadGuard], (guard: SchemeLoadGuard) => {
    expect(guard).toBeTruthy();
  }));
});
