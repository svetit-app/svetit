import { ComponentFixture, TestBed } from '@angular/core/testing';

import { SignTypeDetailDialogComponent } from './sign-type-detail-dialog.component';

describe('SignTypeDetailDialogComponent', () => {
  let component: SignTypeDetailDialogComponent;
  let fixture: ComponentFixture<SignTypeDetailDialogComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ SignTypeDetailDialogComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(SignTypeDetailDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
