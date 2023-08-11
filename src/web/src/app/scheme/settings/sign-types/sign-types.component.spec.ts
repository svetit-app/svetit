import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { SignTypesComponent } from './sign-types.component';

describe('SignTypesComponent', () => {
  let component: SignTypesComponent;
  let fixture: ComponentFixture<SignTypesComponent>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
      declarations: [ SignTypesComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(SignTypesComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
