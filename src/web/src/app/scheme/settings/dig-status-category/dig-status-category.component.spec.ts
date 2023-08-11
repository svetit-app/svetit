import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { DIG_Status_Category_Component } from './dig-status-category.component';

describe('DIG_Status_Category_Component', () => {
  let component: DIG_Status_Category_Component;
  let fixture: ComponentFixture<DIG_Status_Category_Component>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
      declarations: [ DIG_Status_Category_Component ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(DIG_Status_Category_Component);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
